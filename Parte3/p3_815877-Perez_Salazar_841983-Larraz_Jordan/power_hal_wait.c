#include <LPC210X.h>
#include "power_hal_wait.h"

void power_hal_wait(void){
			PCON |= 0x01; 
}

void power_hal_deep_sleep(void){
	EXTWAKE = 6;
	PCON |= 0x02;
	Switch_to_PLL();
}

