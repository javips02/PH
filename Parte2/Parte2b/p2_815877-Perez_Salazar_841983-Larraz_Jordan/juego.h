#ifndef JUEGO_H
#define JUEGO_H
#include <inttypes.h>
#include "temporizador_drv.h" //ahora es de sistema
#include "fifo.h"
void juego_inicializar(void);
void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData);

#endif

