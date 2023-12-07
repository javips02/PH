#ifndef BOTONES_H
#define BOTONES_H
#include <stdint.h>
#include "gestorAlarma.h"
#include "int_externas_hal.h"

static uint8_t estado1=0;
static uint8_t estado2=0;

void esPulsado(uint32_t id_boton);
void botones_ini(void (*funcion_encolar_evento)(), EVENTO_T _miEventoBoton, EVENTO_T _boton1, EVENTO_T _boton2, EVENTO_T _botonTemporizador);
void comprobarEstado(void);


#endif

