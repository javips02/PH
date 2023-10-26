#include <LPC210X.h>
#include "power_hal_wait.h"

void power_hal_wait(void){
			PCON |= 0x01; 
}
