#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H
#include <inttypes.h>
#include <stddef.h>
static void (*callbackToPulsacion)(uint32_t)=NULL;
void eint_init (void (*funcion_pulsar_boton)(uint32_t));
void habilitar_irq_eint1(void);
unsigned int estaPulsadoEint1(void);

void habilitar_irq_eint2(void);
unsigned int estaPulsadoEint2(void);


#endif

