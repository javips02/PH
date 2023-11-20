#include <LPC210X.h>   
#include "linea_serie_drv.h"

static uint8_t buffer[3];
static uint8_t estado;
static uint8_t indice;
static void (*encolar)()=NULL;
static uint8_t bufferEnvios[100];
static uint8_t indice_envios;
void uart0_drv_iniciar(void (*funcion_encolar_evento)()){//// el ioreserva solo lo tiene el planificador
	estado = 0;
	indice = 0;
	gpio_hal_sentido(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
	gpio_hal_escribir(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, 0);
	uart0_hal_iniciar(callback_entrada, linea_serie_drv_continuar_envio);
	encolar=funcion_encolar_evento;
}

void callback_entrada(uint8_t car){
		if(estado == 1){
			if((car != '!' && indice == 3 ) || (indice > 3)){
				gpio_hal_escribir(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, 1);
				estado=2;
				indice=0;
			}else if(car == '!' && indice == 3 ){
					disable_irq();
				  uint32_t valor = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
					encolar(ev_RX_SERIE, valor);
					estado =0;
					indice=0;
					enable_irq();
			}else{
				buffer[indice]=car;
				indice++;
			}
		}
		else if(estado == 2){
			if(car=='$'){
				gpio_hal_escribir(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, 0);
				estado = 1;
			}
		}
		else if (estado ==0){
			if(car=='$'){
				estado = 1;
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
		estado=0;
		disable_irq();
		encolar(ev_TX_SERIE, 0);
		enable_irq();
	}else{
		uart0_hal_enviar(bufferEnvios[indice_envios]);
		indice_envios++;
	}
	
}

