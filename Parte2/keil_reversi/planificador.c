#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "planificador.h"


void planificador(void){
	EVENTO_T *aTratar=NULL;
	uint32_t *data=0;
	FIFO_inicializar(GPIO_OVERFLOW);
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	while(1){
		while(FIFO_extraer(aTratar, data) < 1);
		hello_world_tick_tack();
	}
}
