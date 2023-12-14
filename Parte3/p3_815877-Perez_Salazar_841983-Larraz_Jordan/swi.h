#ifndef SWI_H
#define SWI_H

#include <inttypes.h>
#include <stddef.h>
//Fichero de cabecera para las funciones del SWI
//En este caso, est�n todas implementadas en ARM, por lo que para ver su c�digo bast acon mirar el fichero SWI.s
//Para qeu funcionen correctamente, se han a�adido declaraciones al startup.s

void __swi(0xFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);
uint8_t __swi(0xFC) read_IRQ_bit(void);

#endif
