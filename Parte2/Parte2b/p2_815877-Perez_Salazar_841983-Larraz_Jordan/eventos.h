#ifndef EVENTOS_H
#define EVENTOS_H
#include <inttypes.h>
static uint32_t totalAtendidos;
typedef enum {
	IDVOID =0,
	TIMER1=1,
	EINT1 = 2,
	EINT2 = 3,
	POWER_DOWN=4,
	ev_VISUALIZAR_CUENTA=5,
	ev_LATIDO = 6,
	ev_VISUALIZAR_HELLO = 7,
	ALARMA_OVERFLOW = 8,
	BOTON = 9,
	BOTON_TEMPORIZADOR=10,
}EVENTO_T;

#endif

