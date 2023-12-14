#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H
//Fichero de cabecera para las funciones de linea serie, que se encargan de gestioar la E/S en la uart0 del LPC2105
#include <stddef.h>
#include <inttypes.h>

static void (*callbackToDRV_entrada)(uint8_t caracter)=NULL; //Puntero a la función de callback en el driver para la IRQ de entrada 
static void (*callbackToDRV_salida)()=NULL; //Puntero a la función de callback en el driver para la IRQ de saldia 
void uart0_hal_iniciar( void (*funcion_callback_entrada)(uint8_t car), void (*funcion_callback_salida)()); //Inicializa las IRQ de la UART0 en el LPC2105 y configura las fucniones de callback para el driver
void uart0_hal_enviar(uint8_t car); //Envía un carácter por la uart 

#endif
