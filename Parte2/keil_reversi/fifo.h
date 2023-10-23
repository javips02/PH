#ifndef FIFO_H
#define FIFO_H

#include "gpio_hal.h"
#include "eventos.h"
#include "io_reserva.h"
#include "power_hal_wait.h"
#include <stdint.h>

#define MAX 32 //mover a io_reserva

// Definición de la estructura EVENTO_T
typedef struct {
    EVENTO_T id; // Campo para identificar el evento
		uint8_t procesado; //indica si está procesado (se podría hacer con indices de la cola)?
		uint32_t aux; //Info adicional (no utilizado en esta entrega)
} EVENTO;
EVENTO cqueue_arr[MAX];
int front = -1;
int rear = -1;
// Variable para mantener un registro de estadísticas
uint32_t estadisticasEventos[MAX];
uint32_t eventosEncolados;

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

#endif
