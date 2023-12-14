#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "Watchdog.h"
//inicializa el tiempo del timer y se pone el reset activo
void WD_hal_inicializar(int sec){
	WDTC = (sec * 15000000)/4;
	WDMOD = 3;
	WD_hal_feed();
}
// se le alimenta con una secuencia especial y se activa y desactivan irqs por si acaso si es necesario
void WD_hal_feed(void){
	int irq_activada = read_IRQ_bit();
	disable_irq();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	if(irq_activada==0){
		enable_irq();
	}
	
}
void WD_hal_test(void){
	while(1);
}

