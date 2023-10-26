#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
#include "fifo.h"
#include "hello_world.h"
#include "gpio_hal.h"
#include "io_reserva.h"
#include "temporizador_drv.h"
void planificadorOverflow(void);	
void planificador(void);	
#endif
