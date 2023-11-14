#include <LPC210x.H>                       /* LPC210x definitions */
#include "planificador.h"
#include "Power_management.h"
#include "swi.h"

int main (void) {
	//Para probar las swi
	disable_fiq();
	enable_irq();
	uint32_t status = read_IRQ_bit();//0 activadas, 1 desactivadas
	if ( status != 0) {while (1);}
	disable_irq();
	if ( status != 1) {while (1);}
	
	gpio_hal_iniciar();
	planificador();
	//planificador_Alarm_OF();
}

