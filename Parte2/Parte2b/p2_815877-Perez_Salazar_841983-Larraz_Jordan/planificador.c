#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "planificador.h"


void planificadorOverflow(void){
	FIFO_inicializar(GPIO_OVERFLOW);
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	while(1){
	}
}


void planificador(void){
	EVENTO_T aTratar;
	uint32_t data;
	FIFO_inicializar(GPIO_OVERFLOW);
	alarma_inicializar();
	alarma_activar(POWER_DOWN, USUARIO_AUSENTE*1000, 0);
	botones_ini();
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	static uint8_t iniciada = 0;
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	visualizar_inicializar(JUEGO, JUEGO_BITS);
	juego_inicializar();
	while(1){
		while(FIFO_extraer(&aTratar, &data) < 1){
			power_hal_wait();
		};
			if(aTratar == TIMER1){
				alarma_tratar_evento();
			}
			if(aTratar == POWER_DOWN){
				power_hal_deep_sleep();
				iniciada = 1;
			}
			
		if(aTratar == ev_LATIDO){
			hello_world_tratar_evento();
		}	else if(aTratar == ev_VISUALIZAR_HELLO){
				hello_world_tick_tack();
		}
		else if(aTratar == ev_VISUALIZAR_CUENTA){
			visualizar_jugar(JUEGO, JUEGO_BITS, data);
		}
		else if(aTratar == BOTON){
			if(iniciada == 0){
				alarma_activar(POWER_DOWN, USUARIO_AUSENTE*1000, 0);
				juego_tratar_evento(aTratar, data);
			}else{
				iniciada = 0;
			}	
		}
		else if(aTratar == BOTON_TEMPORIZADOR){
			comprobarEstado();
		}
	}
}
