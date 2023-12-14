#include <LPC210X.h>   
#include "linea_serie_hal.h"

void uart0_ISR (void) __irq;    // Generar interrupción


void uart0_hal_iniciar( void (*funcion_callback_entrada)(uint8_t car), void (*funcion_callback_salida)()){
		PINSEL0 |= 0x5; // Configurar pines P0.0 y P0.1 como TXD0 y RXD0, respectivamente.
		U0LCR=0x80; 		// Configurar el registro de control de línea (LCR) para habilitar el acceso a los registros divisor de la tasa de baudios.
		U0DLL = 97; 		// Configurar el divisor de la tasa de baudios (DLL y DLM) para una tasa de baudios de 9600.
		U0LCR = 0x3;		// Configurar el LCR para deshabilitar el acceso a los registros divisor de la tasa de baudios (una vez zconfigurado en la linea anterior).
		U0IER = 0x3;		// Habilitar interrupciones de recepción y transmisión.
		VICVectAddr4 = (unsigned long)uart0_ISR;          // set interrupt vector in 0 
		VICVectCntl4 = 0x20 | 6; 
		VICIntEnable = VICIntEnable | 0x00000040 ;
		callbackToDRV_entrada=funcion_callback_entrada;		// Configurar funciones de callback para el driver
		callbackToDRV_salida=funcion_callback_salida;
}
void uart0_hal_enviar(uint8_t car){
			U0THR = car;		//Pone un carácter en el registro de salida
}
void uart0_ISR (void) __irq{

	VICVectAddr = 0;				//Petición
	uint8_t cmp = (U0IIR & 0xF) >> 1; 
	
	if(cmp == 2){ //Lectura (desde teclado)
			callbackToDRV_entrada(U0RBR);
	}else if (cmp == 1){ //Escritura (hacia pantalla)
		callbackToDRV_salida();
	}
} 

