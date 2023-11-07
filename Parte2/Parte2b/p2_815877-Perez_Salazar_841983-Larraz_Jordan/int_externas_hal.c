#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "int_externas_hal.h"

void eint1_ISR (void) __irq;
void eint2_ISR (void) __irq;

void eint_init (void (*funcion_pulsar_boton)(uint32_t)) {
	callbackToPulsacion = funcion_pulsar_boton;
	EXTINT =  EXTINT | 2;        // clear interrupt flag (para todas las extint)    	
	VICVectAddr3 = (unsigned long)eint1_ISR;          // set interrupt vector in 1
	PINSEL0 = PINSEL0 & 0xcfffffff;			//Pone bits 31,30,29,28 a 0 (EXTINT 1 y 2) (Table 56 LPC manual) (primero a 0 y después asignar valor)
  PINSEL0 = PINSEL0 | 0x20000000;			// Pone bits 31,30,29,28 a 1,0,1,0 respectivamente (Table 56)
	VICVectCntl3 = 0x20 | 15;  					//Ponemos el bit 7 del VICVectCntl2 a 2, para prio IRQ2 (EINT1)

	EXTINT =  EXTINT | 4;
	VICVectAddr4 = (unsigned long)eint2_ISR;					// set interrupt vector in 2
	PINSEL0 = PINSEL0 & 0x3fffffff;	
  PINSEL0 = PINSEL0 | 0x80000000;
	VICVectCntl4 = 0x20 | 16;	 // Misma prio para el 3 (EINT2) Como ambas tienen misma prio, se atiende por orden de llegada (creo)
	
  VICIntEnable = VICIntEnable | 0x00018000;                  // Enable EXTINT0 Interrupt
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
