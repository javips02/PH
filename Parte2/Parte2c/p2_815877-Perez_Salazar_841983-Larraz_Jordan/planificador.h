#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
#include "fifo.h"
#include "hello_world.h"
#include "gpio_hal.h"
#include "io_reserva.h"
#include "botones.h"
#include "temporizador_drv.h"
#include "gestorAlarma.h"
#include "visualizar.h"
#include "juego.h"
#include "linea_serie_drv.h"
#include "Watchdog.h"


#define USUARIO_AUSENTE 12

void planificadorOverflow(void);	
void planificador(void);	
void planificador_Alarm_OF(void);


#endif
