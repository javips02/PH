#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "int_externas_hal.h"
// variable que se activa al detectar una nueva pulsación

void eint1_ISR (void) __irq;
void eint2_ISR (void) __irq;
void eint_init (void (*funcion_pulsar_boton)(uint32_t)){
	//eint1_nueva_pulsacion = 0;
	EXTINT =  EXTINT | 2;        // clear interrupt flag     	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint1_ISR; 
	// set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
	callbackToPulsacion = funcion_pulsar_boton;
		// 14 is the number of the interrupt assigned. Number 14 is the EINT0 (see table 40 of the LPC2105 user manual  
	PINSEL0 		= PINSEL0 & 0xcfffffff;	//Sets bits 0 and 1 to 0
	PINSEL0 		= PINSEL0 | 0x20000000;					//Enable the EXTINT0 interrupt
	VICVectCntl2 = 0x20 | 15;                   
	EXTINT =  EXTINT | 4;        // clear interrupt flag     	
	VICVectAddr3 = (unsigned long)eint2_ISR;  
	PINSEL0 		= PINSEL0 & 0x3fffffff;	//Sets bits 0 and 1 to 0
	PINSEL0 		= PINSEL0 | 0x80000000;					//Enable the EXTINT0 interrupt
	VICVectCntl3 = 0x20 | 16;                   
  VICIntEnable = VICIntEnable | 0x00018000;   
}

unsigned int estaPulsadoEint1(void){
	EXTINT =  EXTINT | 2; 
	if((EXTINT & 2)==2){
		return 1;
	}else{
	return 0;
	}
};
unsigned int estaPulsadoEint2(void){
	EXTINT =  EXTINT | 4; 
	if((EXTINT & 4)==4){
		return 1;
	}else{
	return 0;
	}
};

void habilitar_irq_eint1(void){
	VICIntEnable = VICIntEnable | 0x00008000;     
}
void habilitar_irq_eint2(void){
	VICIntEnable = VICIntEnable | 0x00010000;     
}


void eint1_ISR (void) __irq {
	VICIntEnClr = 0x00008000;
	EXTINT =  EXTINT | 2;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	callbackToPulsacion(EINT1);

}

void eint2_ISR (void) __irq {
	VICIntEnClr = 0x00010000;
	EXTINT =  EXTINT | 4;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	callbackToPulsacion(EINT2);
}
