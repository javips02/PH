#include "temporizador_drv.h"

 void (*encolarDRV)(EVENTO_T, uint32_t)= NULL; //funcion de callback para encolar evento que se recibirá por parametro
 EVENTO_T id_evento_a_encolar;
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
void temporizador_drv_callbackEncolar() {
	
	encolarDRV(id_evento_a_encolar, 0); //AuxData de momento es 0 default
}
	
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento){
	void (*funcion_callback_ptr)() = temporizador_drv_callbackEncolar;
	encolarDRV = funcion_encolar_evento; 
	id_evento_a_encolar = ID_evento;
	
	temporizador_hal_reloj(periodo, funcion_callback_ptr);
}

uint32_t __SWI_0 (void){
	return temporizador_drv_leer();
}
