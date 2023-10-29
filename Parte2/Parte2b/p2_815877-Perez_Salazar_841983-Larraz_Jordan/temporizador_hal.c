
#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "temporizador_hal.h"

//Definiciones de varibales

static volatile uint64_t timer0_int_count; // n�mero de interrupciones
static volatile uint64_t timer1_int_count;


void timer0_ISR (void) __irq;    // Generar interrupci�n
void timer1_ISR (void) __irq;    // Generar interrupci�n

/* Setup the Timer Counter 0 Interrupt */
void temporizador_hal_iniciar (void) {
	timer0_int_count = 0;	
	timer1_int_count=0;
	// configuration of Timer 0
	T0PR = 0;
	T0MR0 = 0xFFFFFFFE;  								// 1mSec= 15000 - 1 counts .... Menos valor == interupciones m�s seguidas
	T0MCR = 3;                     // Generates an interrupt and resets the count when the value in ticks of MR0 is reached
	// configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
	// 0x20 bit 5 enables vectored IRQs. 
	// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
	VICVectCntl0 = 0x20 | 4;    	
}

void temporizador_hal_empezar(void){
		timer0_int_count = 0;
		T0TCR = 1;         	// Resetea TC
		VICIntEnable = VICIntEnable | 0x00000020 ;
}

uint64_t temporizador_hal_leer(void){
		//return timer0_int_count ;//*T0MR0 + T0TC; Estamos contando s�lamente ticks, no?
	return timer0_int_count * (T0MR0+1) + T0TC;
}

uint64_t temporizador_hal_parar(void){
			T0TCR = 3;
			T0TCR = 0; // When zero, the counters are disabled.
			return temporizador_hal_leer();
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Bajar flag (interrupci�n manejada)
    VICVectAddr = 0;                            // Volvemos a activar VIC para atender pr�ximas interrupciones
}
void timer1_ISR (void) __irq {
    //timer1_int_count++;
	if (callbackToDRV != NULL) {
		callbackToDRV();
	}
    T1IR = 1;                              // Bajar flag (interrupci�n manejada)
    VICVectAddr = 0;                            // Volvemos a activar VIC para atender pr�ximas interrupciones
}

unsigned int timer0_read_int_count(void){
	return timer0_int_count;
};

void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)()){
	 if (periodo == 0) {
        T0TCR = 0; // Detener el temporizador
    }
	VICVectAddr1 = (unsigned long)timer1_ISR;
	VICVectCntl1 = 0x20 | 5;  
	T1TCR = 1;
	VICIntEnable = VICIntEnable | 0x00000010 ;
	T1PR = 0;
	T1MR0 = periodo * 15000 -1;
	T1MCR = 3;

	callbackToDRV = funcion_callback;
}

