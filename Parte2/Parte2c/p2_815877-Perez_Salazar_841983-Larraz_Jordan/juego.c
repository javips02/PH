#include "juego.h"

//static uint32_t cuentaJuego, intervalo;
static uint8_t salida[8][8];
static uint8_t nuevoVector[200];
static TABLERO cuadricula;
void juego_inicializar(void){
    //cuentaJuego=0;
    //intervalo=0;
		static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...

		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
	

}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){
		if(ID_Evento == ev_RX_SERIE){
		char letra1 = (char)(auxData & 0xFF);         // Bits 0-7
    char letra2 = (char)((auxData >> 8) & 0xFF);  // Bits 8-15
    char letra3 = (char)((auxData >> 16) & 0xFF); // Bits 16-23
			if(letra1 == 'B' && letra2 == 'A' && letra3=='T'){
				//empezar a contar
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);

			}
		}else if(ID_Evento == ev_TX_SERIE){
				//parar de contar
				while(1);//probar watchdog
		}
	
    /*if (auxData == EINT1){
        cuentaJuego++;
    } else if (auxData == EINT2 && cuentaJuego > 0 ) { //que no baje de 0 cuenta
        cuentaJuego--;
    }
		FIFO_encolar(ev_VISUALIZAR_CUENTA, cuentaJuego);
    intervalo = temporizador_drv_leer() - intervalo; //update intervalo*/
	
}
void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t){
				int index =0;
				for (size_t i =0; i < 8 ; i++){
					for (size_t j = 0; j < 8; j++) {
						//casos a cubrir: tengo i y j al entrar aqui
						//esquina superior izda caso especial
						if (i==0 && j==0) {
							nuevoVector[index++] = '-';
							nuevoVector[index++] = '|';
						}
						//columna 0: F + numI
						else if (i!=0 && j==0){
							nuevoVector[index++] = '0' + i;
							nuevoVector[index++] = '|';
						}
						//fila 0: C + numJ
						else if (i==0 && j!=0) {
							nuevoVector[index++] = '0' + j;
							nuevoVector[index++] = '|';
						}
						//resto de casos: leer tablero y poner info
						else {
							CELDA escribir = tablero_leer_celda(t,i-1,j-1);
							uint8_t c = celda_color(escribir);
							if (c == 0){
								nuevoVector[index++] = ' ';
								nuevoVector[index++] = '|';
							} else if (c==1){
								nuevoVector[index++] = 'N';
								nuevoVector[index++] = '|';
							} else {
								nuevoVector[index++] = 'B';
								nuevoVector[index++] = '|';
							}
						}
					}
					nuevoVector[index++] = '\n';
				}
				nuevoVector[index++]='\0';
				linea_serie_drv_enviar_array(nuevoVector);				
}

