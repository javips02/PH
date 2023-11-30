#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H

#include <stddef.h>
#include <inttypes.h>

static void (*callbackToDRV_entrada)(uint8_t caracter)=NULL;
static void (*callbackToDRV_salida)()=NULL;
void uart0_hal_iniciar( void (*funcion_callback_entrada)(uint8_t car), void (*funcion_callback_salida)());
void uart0_hal_enviar(uint8_t car);

#endif
