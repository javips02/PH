 
#include "linea_serie_drv.h"

static uint8_t buffer[3];
static uint8_t indice;
static void (*encolar)()=NULL;
static uint8_t bufferEnvios[100];
static uint8_t indice_envios;
static uint32_t evento_uart, evento_uart_bits;
enum Estado {
    IDLE,
    WRITING,
    ERRORCMD
};
enum Estado estado = IDLE;

void uart0_drv_iniciar(void (*funcion_encolar_evento)(), uint32_t evento, uint32_t evento_bits){//// el ioreserva solo lo tiene el planificador
	
	estado = IDLE;
	indice = 0;
	evento_uart = evento;
	evento_uart_bits=evento_bits;
	gpio_hal_sentido(evento, evento_bits, GPIO_HAL_PIN_DIR_OUTPUT);
	gpio_hal_escribir(evento, evento_bits, 0);
	uart0_hal_iniciar(callback_entrada, linea_serie_drv_continuar_envio);
	encolar=funcion_encolar_evento;
}

void callback_entrada(uint8_t car){
		if(estado == WRITING ){
			if((car != '!' && indice == 3 ) || (indice > 3)){
				gpio_hal_escribir(evento_uart, evento_uart_bits, 1);
				estado=ERRORCMD;
				indice=0;
			}else if(car == '!' && indice == 3 ){
					disable_irq();
				  uint32_t valor = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
					encolar(ev_RX_SERIE, valor);
					estado =IDLE;
					indice=0;
					enable_irq();
			}else{
				buffer[indice]=car;
				indice++;
			}
		}
		else if(estado == ERRORCMD){
			if(car=='$'){
				gpio_hal_escribir(evento_uart, evento_uart_bits, 0);
				estado = WRITING;
			}
		}
		else if (estado ==IDLE){
			if(car=='$'){
				estado = WRITING;
			}
		}
}
void linea_serie_drv_enviar_array(uint8_t envio[]){
		memset(bufferEnvios, 0, 100);
		indice_envios=0;
		size_t longitud = strlen((const char *)envio);
    memcpy(bufferEnvios, envio, longitud);
		uart0_hal_enviar(bufferEnvios[indice_envios]);
		indice_envios++;
}

void linea_serie_drv_continuar_envio(void){
	if(bufferEnvios[indice_envios] == '\0'){
		estado=IDLE;
		disable_irq();
		encolar(ev_TX_SERIE, 0);
		enable_irq();
	}else{
		uart0_hal_enviar(bufferEnvios[indice_envios]);
		indice_envios++;
	}
	
}

