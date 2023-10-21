
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "temporizador_hal.h"

//Definiciones de varibales

static volatile uint64_t timer0_int_count; // número de interrupciones
static volatile uint64_t timer1_int_count;


void timer0_ISR (void) __irq;    // Generar interrupción
void timer1_ISR (void) __irq;    // Generar interrupción

/* Setup the Timer Counter 0 Interrupt */
void temporizador_hal_iniciar (void) {
		timer0_int_count = 0;	
		timer1_int_count=0;
		// configuration of Timer 0
		T0PR = 2999;
		T0MR0 = 1000;  								// 1mSec= 15000 - 1 counts .... Menos valor == interupciones más seguidas
    T0MCR = 3;                     // Generates an interrupt and resets the count when the value in ticks of MR0 is reached
		T1MCR = 3;
	
		// configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
		VICVectAddr1 = (unsigned long)timer1_ISR;
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;
		VICVectCntl0 = 0x20 | 5;      	
}

void temporizador_hal_empezar(void){
		T0IR = 1;         	// Clear interrupt flag
		T1TCR = 1;
		VICIntEnable = VICIntEnable | 0x00000010 ;
		VICIntEnable = VICIntEnable | 0x00000020 ;
}

uint64_t temporizador_hal_leer(void){
		return timer0_int_count ;//*T0MR0 + T0TC; Estamos contando sólamente ticks, no?
}
uint64_t temporizador_hal_parar(void){
			T0TCR = 3;
			T0TCR = 0;
			return temporizador_hal_leer();
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Bajar flag (interrupción manejada)
    VICVectAddr = 0;                            // Volvemos a activar VIC para atender próximas interrupciones
}
void timer1_ISR (void) __irq {
    //timer1_int_count++;
	if (callbackToDRV != NULL) {
		callbackToDRV();
	}
    T1IR = 1;                              // Bajar flag (interrupción manejada)
    VICVectAddr = 0;                            // Volvemos a activar VIC para atender próximas interrupciones
}

unsigned int timer0_read_int_count(void){
	return timer0_int_count;
};

void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)()){
	 if (periodo == 0) {
        T0TCR = 0; // Detener el temporizador
    }
	T1MR1 = periodo * 5000 -1;
	callbackToDRV = funcion_callback;
}

