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
		} else if(aTratar == POWER_DOWN){
			power_hal_deep_sleep();
			iniciada = 1;
		} else if(aTratar == ev_LATIDO){
			hello_world_tratar_evento();
		}	else if(aTratar == ev_VISUALIZAR_HELLO){
				hello_world_tick_tack();
		} else if(aTratar == ev_VISUALIZAR_CUENTA){
			visualizar_jugar(JUEGO, JUEGO_BITS, data);
		} else if(aTratar == BOTON){
			if(iniciada == 0){
				alarma_activar(POWER_DOWN, USUARIO_AUSENTE*1000, 0);
				juego_tratar_evento(aTratar, data);
			}else{
				iniciada = 0;
			}	
		} else if(aTratar == BOTON_TEMPORIZADOR){
			comprobarEstado();
		}
	}
}

//Versión del planificador que ignora al usuario y va a ahorro inmediato (se puede mantener despierto o dormir/despertar)
void planificadorPD(void){
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

	static uint32_t tiempoSinInteracc = 0;
		
	while(1){
		while(FIFO_extraer(&aTratar, &data) < 1){
			if (tiempoSinInteracc > USUARIO_AUSENTE*1000){
				gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
        while (1); //generamos estado de error para depurar
			}
			power_hal_wait();
			tiempoSinInteracc = tiempoSinInteracc + temporizador_drv_leer();
		};
		if(aTratar == TIMER1){
			alarma_tratar_evento();
		} else {
			tiempoSinInteracc = 0;
				if(aTratar == POWER_DOWN){
					uint32_t tiempo_antes_PD =temporizador_drv_leer();
					power_hal_deep_sleep();
					uint32_t tiempo_despues_PD = temporizador_drv_leer();
					if (tiempo_despues_PD > tiempo_antes_PD + 7) { //Comprobamos que el timer se pare cuando hacemos powerdown (damos unos ms de tiempo para leer tiempo)
						while(1);
					}
					iniciada = 1;
					tiempoSinInteracc = 0;
			} else if(aTratar == ev_LATIDO){
				hello_world_tratar_evento();
			}	else if(aTratar == ev_VISUALIZAR_HELLO){
				hello_world_tick_tack();
				tiempoSinInteracc = 0;
			} else if(aTratar == ev_VISUALIZAR_CUENTA){
				visualizar_jugar(JUEGO, JUEGO_BITS, data);
				tiempoSinInteracc = 0;
			} else if(aTratar == BOTON){
				tiempoSinInteracc = 0;
				if(iniciada == 0){
					alarma_activar(POWER_DOWN, USUARIO_AUSENTE*100, 0);
					juego_tratar_evento(aTratar, data);
				}else{
					iniciada = 0;
				}	
			} else if(aTratar == BOTON_TEMPORIZADOR){
				comprobarEstado();
			}
		} //fin else
	}
}


void planificador_Alarm_OF(void){ //Overflow
	FIFO_inicializar(GPIO_OVERFLOW);
	alarma_inicializar();
	alarma_activar(POWER_DOWN, USUARIO_AUSENTE*1000, 0);
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	alarma_activar(TIMER1,0x8000000F,0); //Declaramos muchas alarmas para causar overflow
	alarma_activar(EINT1,0x8000000F,0);
	alarma_activar(EINT2,0x8000000F,0);
	alarma_activar(ev_LATIDO,0x8000000F,0);
	alarma_activar(BOTON_TEMPORIZADOR,0x8000000F,0);
	EVENTO_T aTratar;
	uint32_t data;
	while(1){
		FIFO_extraer(&aTratar, &data); //desencolar eventos hasta el de ALARMA_OVERFLOW (que hemos forzado)
		if(aTratar == ALARMA_OVERFLOW){
			gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
       while (1);
		}
	}
}

