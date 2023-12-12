//Modulo de cabecera para la declaracion de eventos reconocibles por el sistema
//Este modulo es visible desde cualquier otro que requiera de su uso, sin restricciones
#ifndef EVENTOS_H
#define EVENTOS_H
#include <inttypes.h>

typedef enum {
	IDVOID =0, //Evento de prueba, no tiene significado
	TIMER1=1, //Evento que indica interrupción del timer1
	EINT1 = 2, //Evento que indica interrupcion del eint1
	EINT2 = 3, //Evento que indica interrupcion del eint2
	POWER_DOWN=4, //Evento que indica que hay que poner el procesador en modo power down
	ev_VISUALIZAR_CUENTA=5, //Evento del modulo hello_world que indica que hay que visualizar en el gpio la cuenta de pulsaciones
	ev_LATIDO = 6, //COMPLETAR
	ev_VISUALIZAR_HELLO = 7, //Evento del módulo hello_world para visualizar por el gpio la secuencia hello world
	ALARMA_OVERFLOW = 8, //Evento del módulo alarmas que indica que se ha superado el maximo numero de alarmas concurrentes en el sistema
	BOTON = 9, //Evento del módulo int_externas que indica la pulsaicn de un boton(eint1 o eint2)
	BOTON_TEMPORIZADOR=10, //Evento del módulo int_externas que indica que hay que comprobar si algyuno de os botones sigue pulsado (periodico hastaqeu dejen de estar pulsados ambos)
	ev_RX_SERIE=11,  //Evento del módulo linea_serie para avisar de que ha llegado un comando por la entrada de la uart
	ev_TX_SERIE=12,  //Evento del módulo int_externas que indica que se ha terminado de imprimir la ultima orden de output por la uart
	ev_confirmarJugada=13  //Evento del módulo juego que indica que hay una jugada pendiente de ocnfirmar (fin de tiempo para cancelar jugada)
}EVENTO_T;

#endif

