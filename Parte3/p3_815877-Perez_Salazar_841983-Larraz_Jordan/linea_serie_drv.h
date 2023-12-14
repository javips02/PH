#ifndef LINEA_SERIE_DRV_H
#define LINEA_SERIE_DRV_H
//Fichero de cabecera para las definiciones de linea_serie_drv, que gestiona la paarte del driver para el serial 
#include "gpio_hal.h"
#include "linea_serie_hal.h"
#include "swi.h"
#include "eventos.h"
#include <stdint.h>
#include <string.h>
void uart0_drv_iniciar(void (*funcion_encolar_evento)(), uint32_t evento, uint32_t evento_bits); //inicializa estructuras y pines para el serial
void callback_entrada(uint8_t car);										//Funci�n de callback para la entrada de datos por el serial (es llamada desde el hal cada vez qeu entra un car�cter)
void linea_serie_drv_enviar_array(uint8_t envio[]);		//Env�a un array por linea_serie (env�a el primer car�cter)
void linea_serie_drv_continuar_envio(void);						//Env�a por el serial el resto del mensaje iniciado por enviar_array; al encontrar un '\0', encola un ev_TX_SERIE
#endif

