#ifndef BOTONES_H
#define BOTONES_H
#include <stdint.h>
#include "gestorAlarma.h"
#include "int_externas_hal.h"

static uint8_t estado1=0; // 0 = Sin pulsar; 1 = pulsado
static uint8_t estado2=0;

void esPulsado(uint32_t id_boton); //Se activa cuando lleg auna IRQ del EINT1 ó EINT2
void botones_ini(void (*funcion_encolar_evento)(), EVENTO_T _miEventoBoton, EVENTO_T _boton1, EVENTO_T _boton2, EVENTO_T _botonTemporizador); //Inicializa estructuras de datos del modulo botones
void comprobarEstado(void); //COmprueba si EINT1 o EINT2 siguen pulsados (no se vuelven a activar las IRQ's de un botón hasta que no se registra una nueva pulsación)


#endif

