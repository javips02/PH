#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "gestorAlarma.h"

static Alarma AlarmasPosibles[MAX_ALARMAS];

static void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData){
	int indice=0;
	for (int i = 0; i < MAX_ALARMAS; i++) {
        if (AlarmasPosibles[i].enUso == 0) {
            indice = i;
					break;
        }	
    }
	  uint32_t periodicidad = retardo & 0x80000000; 
    uint32_t retardoReal = retardo & 0x7FFFFFFF;
		AlarmasPosibles[indice].id = ID_evento;
		AlarmasPosibles[indice].enUso = 1;
		AlarmasPosibles[indice].retardo = retardoReal;
		AlarmasPosibles[indice].auxData = auxData;
		AlarmasPosibles[indice].esPeriodica = periodicidad;
		AlarmasPosibles[indice].contador = 0;
}	
