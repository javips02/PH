#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H
//Definici�n del m�dulo TEMPORIZADOR_DRV, que se encarga de gestionar el timer del temporizador_hal
#include <inttypes.h>
#include "temporizador_hal.h"

//static void *encolarDRV(EVENTO_T, uint32_t);
//extern void (*encolarDRV)(EVENTO_T, uint32_t);


void temporizador_drv_iniciar(void); //Inicializar cronos (todos ellos)
void temporizador_drv_empezar(void); //Empeazar conteo en todos los cronos

uint64_t temporizador_drv_parar(void); 
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento); //Programar timer1 para que interrumpa cada "periodo" ms, acceda a la funcion encolar y encole
uint32_t __swi(0) clock_getus(void); //Sustituta de temporizador_drv_leer, pero con llamada al sistema
#endif
