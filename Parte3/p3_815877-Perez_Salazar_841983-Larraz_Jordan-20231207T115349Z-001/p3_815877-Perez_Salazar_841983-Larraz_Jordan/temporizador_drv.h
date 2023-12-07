#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H
//Definici�n del m�dulo TEMPORIZADOR_DRV independiente del hardware, que se encarga de gestionar el timer del temporizador_hal.
#include <inttypes.h>
#include "eventos.h"
#include "temporizador_hal.h"

//funciones principales que se dedicar a inicializar el modulo de harwdare, empezarlo o pararlo y leer  mediante la llamada al sistema. 
void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_parar(void);
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento);
uint32_t __swi(0) clock_get_us(void);
#endif
