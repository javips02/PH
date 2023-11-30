#ifndef JUEGO_H
#define JUEGO_H
#include <inttypes.h>
#include "temporizador_drv.h" //ahora es de sistema
#include "fifo.h"
#include "conecta_K_2023.h"
#include "entrada.h"
#include "linea_serie_drv.h"
void juego_inicializar(void (*funcion_encolar_evento)());
void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData);
void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t);//preview: 0 = true, 1 = false
void conecta_K_visualizar_tiempo(uint32_t timeToVisualice);
#endif

