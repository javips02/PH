
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "temporizador_hal.h"
#include "temporizador_drv.h"

/* Setup the Timer Counter 0 Interrupt */
void temporizador_drv_iniciar (void) {
		temporizador_hal_iniciar();

}

void temporizador_drv_empezar(void){
		temporizador_hal_empezar();
}

uint64_t temporizador_drv_leer(void){
		return temporizador_hal_leer
}
uint64_t temporizador_drv_parar(void){
		return temporizador_hal_parar();
}


