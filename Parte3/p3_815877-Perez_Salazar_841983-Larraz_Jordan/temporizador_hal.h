#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H
//Definición del módulo temporizador_HAL, que se encarga de la parte hardware del timer

#include <inttypes.h>
#include <stddef.h>

#define TEMPORIZADOR_HAL_TICKS2US(ticks) (ticks * 0.067) //Conversión entre ticks y us para el LPC2105 


static void (*callbackToDRV)()=NULL; //Funcion a la que llamar cada vez que se reciba una interrupcion del timer

void temporizador_hal_iniciar(void); //Inicializa las estructuras de datos de temposirzador
void temporizador_hal_empezar(void); //Inicia el temporizador timer0
uint64_t temporizador_hal_leer(void); //Devuelve el valor en ticks del timer0
uint64_t temporizador_hal_parar(void); //Para el conteo del timer0
void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)()); //Inicializa timer1 para contar un tick cada periodo us


#endif
