#include "visualizar.h"

void visualizar_inicializar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits){
	gpio_hal_sentido(gpio_inicial, bits, GPIO_HAL_PIN_DIR_OUTPUT);
}
void visualizar_jugar(GPIO_HAL_PIN_T gpio_inicial, uint8_t bits,uint32_t cuenta){
	gpio_hal_escribir(gpio_inicial, bits, cuenta);
}

