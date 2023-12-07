#ifndef LINEA_SERIE_DRV_H
#define LINEA_SERIE_DRV_H
#include "gpio_hal.h"
#include "linea_serie_hal.h"
#include "swi.h"
#include "eventos.h"
#include <stdint.h>
#include <string.h>
void uart0_drv_iniciar(void (*funcion_encolar_evento)(), uint32_t evento, uint32_t evento_bits);
void callback_entrada(uint8_t car);
void linea_serie_drv_enviar_array(uint8_t envio[]);
void linea_serie_drv_continuar_envio(void);
#endif

