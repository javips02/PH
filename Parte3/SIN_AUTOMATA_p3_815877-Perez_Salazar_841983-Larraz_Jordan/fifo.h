#ifndef FIFO_H
#define FIFO_H

#include "gpio_hal.h"
#include "eventos.h"

void FIFO_inicializar(uint32_t _evento_fifo, uint32_t _evento_fifo_bits);

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

#endif
