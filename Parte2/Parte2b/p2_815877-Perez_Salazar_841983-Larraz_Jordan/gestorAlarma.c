#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "gestorAlarma.h"
#include "fifo.h"
#include "temporizador_hal.h"

static Alarma AlarmasPosibles[MAX_ALARMAS];

static void alarma_inicializar(){
	for (int i =0;i<MAX_ALARMAS;i++){
		AlarmasPosibles->enUso = 0;
	}
	temporizador_hal_reloj(1,alarma_tratar_evento()); //le  damos t en ms y @funcion_callback
	//Estoy iniciando el conteo de timer1 con esta funcion? ya no me acuerdo. Hay que iniciarlo ya desde esta funcion si no estamos haciendolo con hal_reloj
}

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

static void alarma_tratar_evento() {
	for (int i=0; i<MAX_ALARMAS; i++){
		if (AlarmasPosibles[i].enUso == 1) {
			AlarmasPosibles[i].contador++; //Cada 1 ms aumentamos cuenta de todas las alarmas activas
			if (AlarmasPosibles[i].contador >= AlarmasPosibles[i].retardo){ //Si hay que disparar el evento de la alarma "i"
				FIFO_encolar(AlarmasPosibles->id, AlarmasPosibles->auxData);	//Disparar (siempre)
				if (AlarmasPosibles->esPeriodica != 1){ //Si no es periodica, eliminar
					AlarmasPosibles->enUso = 0;
				}
			}
		}
	}
}