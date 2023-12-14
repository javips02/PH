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
#include "power_drv.h"

//Fichero de cabecera en el que se gestionara la ejecucion del programa identificando los eventos encolados y pasnadolos a los modulos indicados. 

#define USUARIO_AUSENTE 12 // constante que se usara para programar la alarma del powerdown

void planificadorOverflow(void);//version de prueba del planificador en el que se produce overdlow	
void planificador(void);	// version real del planificador en el que se inicializan los modulos y se detectan y procesan eventos	
void planificador_Alarm_OF(void);// version en el que se produce overflow de alarmas posibles


#endif
