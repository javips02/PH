#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H
//Definición del módulo temporizador_HAL, que se encarga de la parte hardware del timer

#include <inttypes.h>
#include "eventos.h"
#include <stddef.h>
//#define TIMER_PCLK 60000000  // Frecuencia de reloj del LPC2105 en Hz
//#define TEMPORIZADOR_HAL_TICKS2US(ticks) ((ticks * 1000000) / TIMER_PCLK)
#define TEMPORIZADOR_HAL_TICKS2US(ticks) (ticks * 0.067)


static void (*callbackToDRV)()=NULL;

void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer(void);
uint64_t temporizador_hal_parar(void);
void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)());
#endif
