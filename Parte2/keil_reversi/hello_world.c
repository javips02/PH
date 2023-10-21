#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "hello_world.h"

void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits){
	gpio = gpio_inicial;
	bits_gpio=gpio;
	cuenta=gpio_inicial;
	temporizador_drv_reloj(10, FIFO_encolar, TIMER1);
}
void hello_world_tick_tack(void){
		gpio_hal_escribir(cuenta, bits_gpio, 1);
}
