#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H
#include <inttypes.h>
void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer();
uint64_t temporizador_drv_parar();
#endif // CELDA_H