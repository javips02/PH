#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "hello_world.h"
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);
GPIO_HAL_PIN_T gpio=0;
uint8_t bits_gpio=0;
uint8_t cuenta=0;

void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits){
	//void (*funcion_encolar_evento)(EVENTO_T, uint32_t) = FIFO_encolar;
	gpio = gpio_inicial;
	bits_gpio=bits;
	cuenta=0;
	
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
	FIFO_encolar(ev_VISUALIZAR_HELLO,cuenta);
}
