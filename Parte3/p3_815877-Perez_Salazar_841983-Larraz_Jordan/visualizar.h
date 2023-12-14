#ifndef VISUALIZAR_H
#define VISUALIZAR_H
#include <inttypes.h>
#include "gpio_hal.h"
//Fichero de cabecera para las funciones relacionadas con la visualización de elementos de juego a través del GPIO (flags de error)
void visualizar_inicializar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits); //Configura los pines del GPIO para juego (pr2)
void visualizar_jugar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits,uint32_t cuenta); //Escribe la cuenta del juego de la pr2 en el GPIO

#endif

