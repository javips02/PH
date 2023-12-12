#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "gpio_hal.h"
#include "gestorAlarma.h"
void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits, void (*funcion_encolar_evento)(), EVENTO_T _eventoHello, EVENTO_T _eventoVisualizarHello);
void hello_world_tick_tack(void);
void hello_world_tratar_evento(void);
#endif
