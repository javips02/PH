#ifndef GPIO_HAL_H
#define GPIO_HAL_H
#include <LPC210X.h>                            
//poner inline!!!!
#include <inttypes.h>

typedef enum{
	GPIO_HAL_PIN_DIR_INPUT,
	GPIO_HAL_PIN_DIR_OUTPUT
}gpio_hal_pin_dir_t;

typedef uint32_t GPIO_HAL_PIN_T;


__inline static void gpio_hal_iniciar(void){
		PINSEL0=0;
		PINSEL1=0;
};
__inline static void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion){
		 

    // Máscara para establecer bits en IODIR
    uint32_t mascara = ((1u << num_bits) - 1) << gpio_inicial;

    if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
        // Establecer los bits en IODIR a 0 para entrada
        IODIR &= ~mascara;
    } else {
        // Establecer los bits en IODIR a 1 para salida
        IODIR |= mascara;
    }
	
};
__inline static uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
    // Leer el valor de los bits indicados desde gpio_inicial y devolver ese valor como un entero.
    gpio_hal_sentido(gpio_inicial, num_bits, 	GPIO_HAL_PIN_DIR_OUTPUT);
    return (IOPIN >> gpio_inicial) & ((1u << num_bits) - 1);
};
		__inline static void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor){
			 gpio_hal_sentido(bit_inicial, num_bits, 	GPIO_HAL_PIN_DIR_INPUT);
    // Crear una máscara para los bits que queremos escribir
    uint32_t mascara = ((1u << num_bits) - 1) << bit_inicial;
    
    // Limpiar los bits que vamos a escribir en el valor original
    IOPIN = (IOPIN & ~mascara);
    
    // Asegurarse de que el valor a escribir no exceda el número de bits indicados
    valor &= ((1u << num_bits) - 1);
    
    // Desplazar el valor a la posición adecuada y escribirlo en IOPIN
    IOPIN |= (valor << bit_inicial);
};

#endif
