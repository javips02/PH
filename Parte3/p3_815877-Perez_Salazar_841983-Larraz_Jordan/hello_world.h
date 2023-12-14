#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "gpio_hal.h"
#include "gestorAlarma.h"

//Fichero de cabecera para el hello world que es un contador binario que se mostrara en el gpio

void hello_world_iniciar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits, void (*funcion_encolar_evento)(), EVENTO_T _eventoHello, EVENTO_T _eventoVisualizarHello);//funcion que inicializa
//las estructuras de datos y demas modulos necesarios oara la ejecucion del programa
void hello_world_tick_tack(void);// funcion que escribe en el gpio
void hello_world_tratar_evento(void);// funcion que encola el evento que se le ha pasado en el constructor
#endif
