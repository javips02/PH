#ifndef BOTONES_H
#define BOTONES_H
#include <stdint.h>
#include <inttypes.h>
#include "gestorAlarma.h"
#include "int_externas_hal.h"

static uint8_t estado1=0;
static uint8_t estado2=0;

void esPulsado(uint32_t id_boton);
void botones_ini(void);
void comprobarEstado(void);


#endif

