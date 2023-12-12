#include "planificador.h"


void planificadorOverflow(void){
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, FIFO_encolar, ev_LATIDO, ev_VISUALIZAR_HELLO);
	while(1){
	}
}


void planificador(void){
	
	//inicializaciones de los modulos usados en la ejecucion de un juego conecta_K con el HelloWorldContador
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	alarma_inicializar(FIFO_encolar, TIMER1);
	WD_hal_inicializar(1);
	//alarma_activar(POWER_DOWN, USUARIO_AUSENTE*1000, 0);
	uart0_drv_iniciar(FIFO_encolar, GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS);
	botones_ini(FIFO_encolar, BOTON, EINT1, EINT2, BOTON_TEMPORIZADOR);
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	static uint8_t iniciada = 0;
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, FIFO_encolar, ev_LATIDO, ev_VISUALIZAR_HELLO);
	visualizar_inicializar(JUEGO, JUEGO_BITS);
	juego_inicializar(GPIO_JUEGO_ERROR, GPIO_JUEGO_ERROR_BITS);
	
	EVENTO_T aTratar;
	uint32_t data;
	// se lee el proximo evento a tratar de la fifo, se ira modo idle si no hay eventos por tratar o a una funcion para tratar dicho evento 
	//ademas se alimenta al watchdog para postergar el reset
	while(1){
		while(FIFO_extraer(&aTratar, &data) < 1){
			power_drv_wait();
		};
		WD_hal_feed();
			if(aTratar == TIMER1){
				alarma_tratar_evento();
			}
			if(aTratar == POWER_DOWN){
				power_drv_deep_sleep();
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
		else if(aTratar==ev_RX_SERIE){
			juego_tratar_evento(aTratar, data);
		}
		else if(aTratar == ev_TX_SERIE){
			juego_tratar_evento(aTratar, data);
		}
		else if (aTratar == ev_confirmarJugada){
			juego_tratar_evento(aTratar, data);
		}
	}
}








void planificador_Alarm_OF(void){ //Overflow
	FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);
	alarma_inicializar(FIFO_encolar, TIMER1);
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
