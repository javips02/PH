#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H
#include <inttypes.h>
void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer();
uint64_t temporizador_hal_parar();
#endif // CELDA_H