#include "juego.h"

static uint32_t cuentaJuego, intervalo;
static uint8_t salida[8][8];
static uint8_t nuevoVector[200];
void juego_inicializar(void){
    cuentaJuego=0;
    intervalo=0;
		static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...
		static uint8_t autoJugada[3][3];
		TABLERO cuadricula;
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
	

}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){
	
	
		int index = 0; // Índice para el nuevo vector
		for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
						switch (salida[i][j]) {
								case 0x11:
										nuevoVector[index++] = 'N';
										nuevoVector[index++] = '|';
										break;
								case 0x22:
										nuevoVector[index++] = 'B';
										nuevoVector[index++] = '|';
										break;
								case 0x00:
										nuevoVector[index++] = ' ';
										nuevoVector[index++] = '|';
										
										break;
								case 0xFC:
										nuevoVector[index++] = '-';
										nuevoVector[index++] = '|';
										break;
								case 0xF0:
								case 0xC0:
										nuevoVector[index++] = '0';
										nuevoVector[index++] = '|';
										break;
								case 0xF1:
								case 0xC1:
										nuevoVector[index++] = '1';
										nuevoVector[index++] = '|';
										break;
								case 0xF2:
								case 0xC2:
										nuevoVector[index++] = '2';
										nuevoVector[index++] = '|';
										break;
								case 0xF3:
								case 0xC3:
										nuevoVector[index++] = '3';
										nuevoVector[index++] = '|';
										break;
								case 0xF4:
								case 0xC4:
										nuevoVector[index++] = '4';
										nuevoVector[index++] = '|';
										break;
								case 0xF5:
								case 0xC5:
										nuevoVector[index++] = '5';
										nuevoVector[index++] = '|';
										break;
								case 0xF6:
								case 0xC6:
										nuevoVector[index++] = '6';
										nuevoVector[index++] = '|';
										break;
								case 0xF7:
								case 0xC7:
										nuevoVector[index++] = '7';
										nuevoVector[index++] = '|';
										break;
						}
				}
				nuevoVector[index++] = '\n';
		}

		nuevoVector[index++]='\0';
		linea_serie_drv_enviar_array(nuevoVector);
    /*if (auxData == EINT1){
        cuentaJuego++;
    } else if (auxData == EINT2 && cuentaJuego > 0 ) { //que no baje de 0 cuenta
        cuentaJuego--;
    }
		FIFO_encolar(ev_VISUALIZAR_CUENTA, cuentaJuego);
    intervalo = temporizador_drv_leer() - intervalo; //update intervalo*/
	
}

