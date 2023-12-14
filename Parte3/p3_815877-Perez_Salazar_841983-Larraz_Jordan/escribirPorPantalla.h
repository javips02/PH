#ifndef ESCRIBIRPORPANTALLA_H
#define ESCRIBIRPORPANTALLA_H
#include <inttypes.h>
#include "linea_serie_drv.h"
#include "conecta_K_2023.h"
#include "temporizador_drv.h" //ahora es de sistema


//Fichero que se ha dividio de juego.c para hacerlo mas claro. las funciones enviaran a la uart los buffers necesarios con la informacion a printear


void inicializarPartida(uint8_t *nuevoVector);//se escibe bienvenido a partida
void mensajeGanador(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector);
void intToStr(uint32_t timeToVisualice,  uint8_t numToStr[15]);//se pasan los tiempos a cadenas
void finalizarPartida(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector, uint32_t tiempoTotalPartidaIni, 
											uint32_t tiempoTotalHayLinea, uint32_t tiempoTotalHumano, uint32_t numCallsHayLinea, uint32_t numJugadasHumano);//se finaliza partida con victoria(res==1) o rendicion(res==2)
	// y se imprimen con los tiempos
void jugadaNoValida(uint8_t *nuevoVector);// se imprime que la jugada no es valida
void comandoNoValido(uint8_t *nuevoVector);// se imprime que el comando no es valido
void movimientoCancelado(uint8_t *nuevoVector);// se imprime que el movimiento ha sido cancelado pulsando eint1
void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t, uint8_t *nuevoVector, uint8_t previewMode, uint8_t jugadaActual[], uint8_t jugadorActual);// se imprime el tablero por pantalla. 
void partidaEmpatada(uint8_t *nuevoVector);
#endif

