#include "hello_world.h"

GPIO_HAL_PIN_T gpio=0;
uint8_t bits_gpio=0;
uint8_t cuenta=0;
static void (*encolar)()=NULL;

void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits, void (*funcion_encolar_evento)()){
	//void (*funcion_encolar_evento)(EVENTO_T, uint32_t) = FIFO_encolar;
	gpio = gpio_inicial;
	bits_gpio=bits;
	cuenta=0;
	encolar = funcion_encolar_evento;
	
	gpio_hal_sentido(gpio_inicial, bits, GPIO_HAL_PIN_DIR_OUTPUT);
	//temporizador_drv_reloj(10, funcion_encolar_evento, TIMER1);
	static uint32_t ret = 0x8000000A;
	alarma_activar(ev_LATIDO,ret,0);
}
void hello_world_tick_tack(void){
		gpio_hal_escribir(gpio, bits_gpio, cuenta);
		cuenta++;
}
void hello_world_tratar_evento(void){
	encolar(ev_VISUALIZAR_HELLO,0);
}
