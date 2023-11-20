#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "Watchdog.h"

void WD_hal_inicializar(int sec){
	WDTC = sec * 15000;
	WDMOD = 3;
}
void WD_hal_feed(void){
	disable_irq();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	enable_irq();
}
void WD_hal_test(void);

