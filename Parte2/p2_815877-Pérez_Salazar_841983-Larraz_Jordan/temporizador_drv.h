#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H
//Definición del módulo TEMPORIZADOR_DRV, que se encarga de gestionar el timer del temporizador_hal
#include <inttypes.h>
void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);
#endif
