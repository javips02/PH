#ifndef POWER_DRV_H
#define POWER_DRV_H

void power_drv_wait(void); //Llama a las funciones de hal que activan el estado de idle en el procesador
void power_drv_deep_sleep(void); //Llama a las fucniones de hal que activan el estado de deep_sleep en el procesador

#endif

