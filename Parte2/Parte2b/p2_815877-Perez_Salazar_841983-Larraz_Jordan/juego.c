#include "juego.h"

uint32_t cuentaJuego, intervalo;
static uint32_t Tini = 0;
void juego_inicializar(void){
    cuentaJuego=0;
    intervalo=0;
		Tini = temporizador_drv_leer();
}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){
    if (auxData == EINT1){
        cuentaJuego++;
    } else if (auxData == EINT2 && cuentaJuego > 0 ) { //que no baje de 0 cuenta
        cuentaJuego--;
    }
		FIFO_encolar(ev_VISUALIZAR_CUENTA, cuentaJuego);
    intervalo = temporizador_drv_leer() - Tini; //update intervalo
}

