#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H
//Definici�n del m�dulo TEMPORIZADOR_DRV, que se encarga de gestionar el timer del temporizador_hal
#include <inttypes.h>
#include "eventos.h"
#include "fifo.h"
#include "temporizador_hal.h"

static void (encolarDRV)(EVENTO_T, uint32_t);
static EVENTO_T id_evento_a_encolar;

void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento);
#endif
