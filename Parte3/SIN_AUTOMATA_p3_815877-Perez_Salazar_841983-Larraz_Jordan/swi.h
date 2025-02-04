#ifndef SWI_H
#define SWI_H

#include <inttypes.h>
#include <stddef.h>

//uint8_t __swi(1) read_IRQ_bit(void);

void __swi(0xFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);
uint8_t __swi(0xFC) read_IRQ_bit(void);

#endif
