#ifndef JUEGO_H
#define JUEGO_H
#include <inttypes.h>
#include "entrada.h"
#include "gestorAlarma.h"
#include "escribirPorPantalla.h"
void juego_inicializar(uint32_t _evento_juego_error, uint32_t _evento_juego_error_bits);
void juego_ini_primera(void);
void juego_ini(void);
void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData);
//void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t);//preview: 0 = true, 1 = false
//void jugadaNoValida(void);
//void comandoNoValido(void);
#endif

