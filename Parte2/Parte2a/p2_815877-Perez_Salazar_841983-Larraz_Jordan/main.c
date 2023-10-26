#include <LPC210x.H>                       /* LPC210x definitions */
#include "planificador.h"
#include "Power_management.h"

int main (void) {
	temporizador_drv_iniciar();
	temporizador_hal_empezar();
	gpio_hal_iniciar();
	planificador();
}

