#ifndef POWER_HAL_WAIT_H
#define POWER_HAL_WAIT_H

void power_hal_wait(void); //Configura el estado de espera en el LPC2105
void power_hal_deep_sleep(void); //Configura el estado de power down en el LPC2105
extern void Switch_to_PLL(void); //  phase-locked loop (PLL)
#endif

