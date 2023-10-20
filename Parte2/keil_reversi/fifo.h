#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H

#include "gpio_hal.h"
#include "eventos.h"

#include <stdint.h>

//typedef struct Evento;

#define MAX 32 //mover a io_reserva

// Definición de la estructura EVENTO_T
typedef struct {
    EVENTO_T id_evento; // Campo para identificar el evento
		uint32_t stats;
} EVENTO;


int cqueue_arr[MAX];
int front = -1;
int rear = -1;


// Variable para mantener un registro de estadísticas
uint32_t estadisticasEventos[MAX];

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

#endif