#include <LPC210x.H>                       /* LPC210x definitions */
#include "planificador.h"
#include "Power_management.h"


int main (void) {
	gpio_hal_iniciar();
	planificador();
	//planificador_Alarm_OF();
}

