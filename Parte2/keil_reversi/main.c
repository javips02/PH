                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "planificador.h"
#include "Power_management.h"

// Nota: wait es una espera activa. Se puede eliminar poniendo el procesador en modo iddle. Probad a hacerlo
/*void wait (void)  {                          wait function 
  unsigned int i;

  i = timer0_read_int_count(); // reads the number of previous timer IRQs
  while ((i + 10) != timer0_read_int_count());               waits for 10 interrupts, i.e. 50ms 
}
*/
	int main (void) {
	temporizador_drv_iniciar();
	temporizador_hal_empezar();
	gpio_hal_iniciar();
	planificador();
	
 
}


//int main (void) {
//  	
//  
//	
//}
