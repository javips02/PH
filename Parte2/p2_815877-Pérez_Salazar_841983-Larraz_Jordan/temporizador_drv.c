
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "temporizador_hal.h"
#include "temporizador_drv.h"

/* Setup the Timer Counter 0 Interrupt */
void temporizador_drv_iniciar (void) { //Genera timer 
	temporizador_hal_iniciar();
}

void temporizador_drv_empezar(void){
		temporizador_hal_empezar();
}

uint64_t temporizador_drv_leer(void){
	uint64_t lectura= temporizador_hal_leer();	 
	return  TEMPORIZADOR_HAL_TICKS2US(lectura);
}
uint64_t temporizador_drv_parar(void){
		return temporizador_hal_parar();
}


