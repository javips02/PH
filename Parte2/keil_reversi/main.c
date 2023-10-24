#include <LPC210x.H>                       /* LPC210x definitions */
#include "fifo.h"
#include "temporizador_drv.h"

//	int main (void) {
//	temporizador_drv_iniciar();
//	temporizador_hal_empezar();
//	gpio_hal_iniciar();
//	planificador();
//}


int main (void) {
	FIFO_inicializar(GPIO_OVERFLOW);
	temporizador_drv_iniciar(); //generamos timers (0 y 1)
	temporizador_drv_reloj(10, FIFO_encolar, TIMER1); //programamos irq TIMER1
	temporizador_drv_empezar(); //iniciamos cuenta de timers 0 y 1
	while(1){} //esperamos a que haya overflow (nos quedaremos en FIFO_encolar en un while 1 con bit d verflow levantado	
}
