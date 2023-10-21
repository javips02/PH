#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "gpio_hal.h"
#include "temporizador_drv.h"
#include "fifo.h"
#include "eventos.h"
GPIO_HAL_PIN_T gpio;
uint8_t bits_gpio;
uint8_t cuenta;
void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits);
void hello_world_tick_tack(void);
#endif
