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
	hello_world_iniciar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	uint32_t res1 = temporizador_drv_leer();
	static uint32_t resTiempoFInal;
	uint8_t cortar = 0;
	while(1){
		if (cortar < 20) {
			while(FIFO_extraer(&aTratar, &data) < 1);
			uint64_t res1 = temporizador_drv_leer();
			hello_world_tick_tack();
			cortar ++;
		}
		else{
			uint64_t reloj = temporizador_drv_leer();
			resTiempoFInal = reloj-res1;
			uint32_t total = FIFO_estadisticas(IDVOID);
			gpio_hal_sentido(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, GPIO_HAL_PIN_DIR_INPUT);
			uint32_t n = gpio_hal_leer(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
			while(resTiempoFInal!=0);
		}
	}
}
