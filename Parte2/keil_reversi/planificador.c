#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "planificador.h"



void planificador(void){
	FIFO_inicializar(GPIO_OVERFLOW);
	EVENTO_T *aTratar;
	uint32_t *data;
	while(FIFO_extraer(aTratar, data) < 1){
		hello_world_tick_tack();
	};
}
