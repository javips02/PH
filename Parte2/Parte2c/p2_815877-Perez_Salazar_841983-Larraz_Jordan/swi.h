#ifndef SWI_H
#define SWI_H

#include <inttypes.h>
#include <stddef.h>

void __swi(0xFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);
uint8_t __swi(0xFC) read_IRQ_bit(void); //Hecha en ensamblador al final, mirar en SWI.s como el resto
//Nota: no incluimos enable_fiq, porque en este proyecto no vamos a usar las interrupciones FIQ. Por tanto,
//bastará con deshabilitarlas al comenzar la ejecución.
#endif
