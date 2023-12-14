#ifndef GESTORALARMA_H
#define GESTORALARMA_H

#include "temporizador_drv.h"

//Fichero de cabecera para las alarmas que encolaran eventos

void alarma_inicializar(void (*funcion_encolar_evento)(), EVENTO_T _miEventoTimer);// constructor para la alarma en el que se le pasa la funcion de encolar y el evento a encolar
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);//funcion que se utilizara en otros modulos para activar una alarma
void alarma_tratar_evento(void); //llamada desde IRQ timer1

#endif

