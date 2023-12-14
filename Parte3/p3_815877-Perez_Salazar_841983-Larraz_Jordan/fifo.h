#ifndef FIFO_H
#define FIFO_H

#include "gpio_hal.h"
#include "eventos.h"
//Fichero de cabecera para la cola que se va utilizar para los eventos
void FIFO_inicializar(uint32_t _evento_fifo, uint32_t _evento_fifo_bits);// se inicializan variables 

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);// se extrae el evento encolado que toque

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);// funcion para encolar eventos

uint32_t FIFO_estadisticas(EVENTO_T ID_evento);// funcion para acceder a las estadisticas de la cola fifo

#endif
