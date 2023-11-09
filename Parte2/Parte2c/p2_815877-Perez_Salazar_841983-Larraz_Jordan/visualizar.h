#ifndef VISUALIZAR_H
#define VISUALIZAR_H
#include <inttypes.h>
#include "temporizador_drv.h" //ahora es de sistema
#include "eventos.h"
#include "gpio_hal.h"
#include "io_reserva.h"
void visualizar_inicializar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits);
void visualizar_jugar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits,uint32_t cuenta);

#endif

