#ifndef ESCRIBIRPORPANTALLA_H
#define ESCRIBIRPORPANTALLA_H
#include <inttypes.h>
#include "linea_serie_drv.h"
#include "conecta_K_2023.h"
#include "temporizador_drv.h" //ahora es de sistema
void intToStr(uint32_t timeToVisualice,  uint8_t numToStr[15]);
void finalizarPartida(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector, uint32_t tiempoTotalPartidaIni, 
											uint32_t tiempoTotalHayLinea, uint32_t tiempoTotalHumano, uint32_t numCallsHayLinea, uint32_t numJugadasHumano);
void jugadaNoValida(uint8_t *nuevoVector);
void comandoNoValido(uint8_t *nuevoVector);
void movimientoCancelado(uint8_t *nuevoVector);
void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t, uint8_t *nuevoVector, uint8_t previewMode, uint8_t jugadaActual[], uint8_t jugadorActual);
#endif

