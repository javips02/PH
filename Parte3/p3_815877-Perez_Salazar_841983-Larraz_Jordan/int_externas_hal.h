//Modulo de Interrupciones externas de botones (EINT1 y EINT2)
#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H
#include <inttypes.h>
#include <stddef.h>
static void (*callbackToPulsacion)(uint32_t)=NULL; //Funcion a la que llamar cada vez que interrumpe eint1 o eint2
void eint_init (void (*funcion_pulsar_boton)(uint32_t)); //Inicializa las estructuras de datos que necesita el modulo
void habilitar_irq_eint1(void); //Activa la sinterrupciones del eint1
unsigned int estaPulsadoEint1(void); //Comprueba si sigue pulsado eint1
void habilitar_irq_eint2(void); //Activa la sinterrupciones del eint2
unsigned int estaPulsadoEint2(void); //Comprueba si sigue pulsado eint2


#endif

