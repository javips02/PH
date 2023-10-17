#include "conecta_K_2023.h"
#include "temporizador_drv.h"

// MAIN 
int main (void) {
	//inicializar sistema
	//... practica 2
	//Provisional: iniciar timer para comprobar funcionamiento y depurar:
	
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	uint64_t ticks;
	ticks	= temporizador_drv_leer();
	while((ticks+1) != temporizador_drv_leer()) {} //de aquí no pasamos hasta qeu funcione bien el timer
	//jugar
	ticks = 0;
	conecta_K_jugar();
	
	while(1); //no hay S.O., no se retorna
}
