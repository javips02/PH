
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "temporizador_hal.h"

#define TIMER_PCLK 60000000  // Frecuencia de reloj del LPC2105 en Hz

#define TEMPORIZADOR_HAL_TICKS2US(ticks) ((ticks * 1000000) / TIMER_PCLK)

// variable para contabilizar el n�mero de interrupciones
static volatile unsigned int timer0_int_count = 0;

void timer0_ISR (void) __irq;    // Generate Interrupt 

/* Setup the Timer Counter 0 Interrupt */
void temporizador_hal_iniciar (void) {
		timer0_int_count = 0;	
	// configuration of Timer 0
		T0MR0 = 2999;  
    T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;                   

}

void temporizador_hal_empezar(void){
		T0IR = 1;                              // Clear interrupt flag
		VICIntEnable = VICIntEnable | 0x10
}

uint64_t temporizador_hal_leer(void){
		return timer0_int_count*T0MR0 + T0TC;
}
uint64_t temporizador_hal_parar(void){
			T0TCR = 3;
			T0TCR = 1;
			return temporizador_hal_leer();
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

unsigned int timer0_read_int_count(void){
	return timer0_int_count;
};

