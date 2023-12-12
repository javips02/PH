#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "swi.h"
void WD_hal_inicializar(int sec);
void WD_hal_feed(void);
void WD_hal_test(void);

#endif

