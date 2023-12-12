#include <LPC210X.h>   
#include "linea_serie_hal.h"

void uart0_ISR (void) __irq;    // Generar interrupción


void uart0_hal_iniciar( void (*funcion_callback_entrada)(uint8_t car), void (*funcion_callback_salida)()){
		PINSEL0 |= 0x5;
		U0LCR=0x80;
		U0DLL = 97;
		U0LCR = 0x3;
		U0IER = 0x3;
		VICVectAddr4 = (unsigned long)uart0_ISR;          // set interrupt vector in 0 
		VICVectCntl4 = 0x20 | 6; 
		VICIntEnable = VICIntEnable | 0x00000040 ;
		callbackToDRV_entrada=funcion_callback_entrada;
		callbackToDRV_salida=funcion_callback_salida;
}
void uart0_hal_enviar(uint8_t car){
			U0THR = car;
}
void uart0_ISR (void) __irq{

	VICVectAddr = 0;
	uint8_t cmp = (U0IIR & 0xF) >> 1;
	
	if(cmp == 2){
			callbackToDRV_entrada(U0RBR);
	}else if (cmp == 1){
		callbackToDRV_salida();
	}
} 

