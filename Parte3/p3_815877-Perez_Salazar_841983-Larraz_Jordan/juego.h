#ifndef JUEGO_H
#define JUEGO_H
#include <inttypes.h>
#include "eventos.h"

//Fichero juego.h que inicializa las estructuras de datos. Recibira eventos del planificador y gestionara los diferentes eventos
// para ejecutar un juego de Conecta 4 en el que el jugador se podra rendir, hacer jugadas y cancelarlas antes de hacerlas efectivas, y ejecutar movimientos para ganar. 


void juego_inicializar(uint32_t _evento_juego_error, uint32_t _evento_juego_error_bits);//inicializar? las distntas estructuras de datos a usar. 
void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData);//tratara eventos y llevar? a cabo una acci?n determinada.

#endif

