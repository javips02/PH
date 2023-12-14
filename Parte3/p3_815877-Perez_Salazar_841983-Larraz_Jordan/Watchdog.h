#ifndef WATCHDOG_H
#define WATCHDOG_H
//fichero WATCHDOG que genera un temporizador en unos determinados que generara un reset si no se le ha alimentado en el periodo
#include "swi.h"
void WD_hal_inicializar(int sec);//inicializa el watchdog
void WD_hal_feed(void);// se le alimenta para prolongar el reset
void WD_hal_test(void);//funcion de test que hace while 1 para comprobar que funciona

#endif

