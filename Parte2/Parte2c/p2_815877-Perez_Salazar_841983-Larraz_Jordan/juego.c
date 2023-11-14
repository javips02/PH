#include "juego.h"

static uint32_t cuentaJuego, intervalo;
void juego_inicializar(void){
    cuentaJuego=0;
    intervalo=0;
}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){
    if (auxData == EINT1){
        cuentaJuego++;
    } else if (auxData == EINT2 && cuentaJuego > 0 ) { //que no baje de 0 cuenta
        cuentaJuego--;
    }
		FIFO_encolar(ev_VISUALIZAR_CUENTA, cuentaJuego);
    intervalo = clock_getus() - intervalo; //update intervalo
}

