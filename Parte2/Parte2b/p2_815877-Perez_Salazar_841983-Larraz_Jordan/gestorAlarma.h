#ifndef GESTORALARMA_H
#define GESTORALARMA_H

#include "temporizador_drv.h"
#include "fifo.h"
#define MAX_ALARMAS 4

typedef struct {
    EVENTO_T id; // Campo para identificar el evento
		uint8_t enUso;
		uint32_t retardo;
		uint32_t auxData;
		uint32_t esPeriodica;
		uint32_t contador;
} Alarma;

void alarma_inicializar(void);
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);
void alarma_tratar_evento(void); //llamada desde IRQ timer1

#endif

