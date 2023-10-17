//#ifndef TEMPORIZADOR_HAL_H
//#define TEMPORIZADOR_HAL_H
//Definici�n del m�dulo temporizador_HAL, que se encarga de la parte hardware del timer
#include <inttypes.h>
void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer(void);
uint64_t temporizador_hal_parar(void);
//#endif
