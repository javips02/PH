#include "gestorAlarma.h"
#define MAX_ALARMAS 4

typedef struct {
    EVENTO_T id; // Campo para identificar el evento
		uint8_t enUso;
		uint32_t retardo;
		uint32_t auxData;
		uint8_t esPeriodica;
		uint32_t contador;
} Alarma;//tipo de dato alarma con un evento que encolara, un campo para saber si esta siendo usada, el retardo para que salte la alarma, el auxData que encolara, un int para saber si es periodica
// y el contador que lleva. 

static Alarma AlarmasPosibles[MAX_ALARMAS];//vector de alarmas
static void (*encolar)()=NULL;// funcion de fifo encolar que se asociara en el constructor
EVENTO_T miEventoTimer;//el evebto de la propia alarma

void alarma_inicializar(void (*funcion_encolar_evento)(), EVENTO_T _miEventoTimer){
	for (int i =0;i<MAX_ALARMAS;i++){
		AlarmasPosibles[i].enUso = 0;
	}
	miEventoTimer=_miEventoTimer;
	encolar = funcion_encolar_evento;
	temporizador_drv_reloj(1,encolar, miEventoTimer);
	//Estoy iniciando el conteo de timer1 con esta funcion? ya no me acuerdo. Hay que iniciarlo ya desde esta funcion si no estamos haciendolo con hal_reloj
}

void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData){// se busca un hueco para iniciar la alarma y sino hay salta overflow
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
		encolar(ALARMA_OVERFLOW,0);
	}else{
		uint32_t periodicidad = (retardo >> 31) & 0x01; 
    uint32_t retardoReal = retardo & 0x7FFFFFFF;
		
		AlarmasPosibles[indice].id = ID_evento;
		if(retardo == 0){// para saber si hay q pararla
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
void alarma_tratar_evento(void) {// aumenta la cuenta de cada alarma
	for (int i=0; i<MAX_ALARMAS; i++){
		if (AlarmasPosibles[i].enUso == 1) {
			AlarmasPosibles[i].contador++; //Cada 1 ms aumentamos cuenta de todas las alarmas activas
			if (AlarmasPosibles[i].contador >= AlarmasPosibles[i].retardo){ //Si hay que disparar el evento de la alarma "i"
					encolar(AlarmasPosibles[i].id, AlarmasPosibles[i].auxData);	//Disparar (siempre)
				if (AlarmasPosibles[i].esPeriodica != 1){ //Si no es periodica, eliminar
					AlarmasPosibles[i].enUso = 0;
				}else{
					AlarmasPosibles[i].contador = 0;
				}
			}
		}
	}
}


