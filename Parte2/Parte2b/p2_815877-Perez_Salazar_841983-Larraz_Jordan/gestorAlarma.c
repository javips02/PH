
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "gestorAlarma.h"

static Alarma AlarmasPosibles[MAX_ALARMAS];
void alarma_inicializar(void){
	for (int i =0;i<MAX_ALARMAS;i++){
		AlarmasPosibles[i].enUso = 0;
	}
	
	void (*funcion_encolar_evento)(EVENTO_T, uint32_t) = FIFO_encolar;
	temporizador_drv_reloj(1,funcion_encolar_evento, TIMER1);
	//Estoy iniciando el conteo de timer1 con esta funcion? ya no me acuerdo. Hay que iniciarlo ya desde esta funcion si no estamos haciendolo con hal_reloj
}

void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData){
	int indice=-1;
	uint8_t encontrado = 0;
	for (int i = 0; i < MAX_ALARMAS; i++) {
        if (AlarmasPosibles[i].id == ID_evento) {
            indice = i;
						encontrado = 1;	
						break;
        }			
  }
	for (int i = 0; i < MAX_ALARMAS && encontrado == 0; i++) {
        if (AlarmasPosibles[i].enUso == 0) {
            indice = i;
					break;
        }			
    }
	if(indice == -1){
		FIFO_encolar(ALARMA_OVERFLOW,0);
	}else{
		uint32_t periodicidad = (retardo >> 31) & 0x01; 
    uint32_t retardoReal = retardo & 0x7FFFFFFF;
		
		AlarmasPosibles[indice].id = ID_evento;
		if(retardo == 0){
		AlarmasPosibles[indice].enUso=0;
		}else{
			AlarmasPosibles[indice].enUso = 1;
		}
		AlarmasPosibles[indice].retardo = retardoReal;
		AlarmasPosibles[indice].auxData = auxData;
		AlarmasPosibles[indice].esPeriodica = periodicidad;
		AlarmasPosibles[indice].contador = 0;
	}
}	
void alarma_tratar_evento(void) {
	for (int i=0; i<MAX_ALARMAS; i++){
		if (AlarmasPosibles[i].enUso == 1) {
			AlarmasPosibles[i].contador++; //Cada 1 ms aumentamos cuenta de todas las alarmas activas
			if (AlarmasPosibles[i].contador >= AlarmasPosibles[i].retardo){ //Si hay que disparar el evento de la alarma "i"
				FIFO_encolar(AlarmasPosibles[i].id, AlarmasPosibles[i].auxData);	//Disparar (siempre)
				if (AlarmasPosibles[i].esPeriodica != 1){ //Si no es periodica, eliminar
					AlarmasPosibles[i].enUso = 0;
				}else{
					AlarmasPosibles[i].contador = 0;
				}
			}
		}
	}
}


