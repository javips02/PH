#include "fifo.h"
#include "swi.h"
#include <stdint.h>

#define MAX 32 //mover a io_reserva

// Definici?n de la estructura EVENTO_T
typedef struct {
    EVENTO_T id; // Campo para identificar el evento
		uint8_t procesado;
		uint32_t aux;
} EVENTO;
volatile static EVENTO cqueue_arr[MAX];
volatile static int front = -1;
volatile static int rear = -1;
// Variable para mantener un registro de estad?sticas
volatile static uint32_t estadisticasEventos[MAX];
volatile static uint32_t eventosEncolados;
static uint32_t evento_fifo, evento_fifo_bits;
volatile static uint32_t totalAtendidos;

void FIFO_inicializar(uint32_t _evento_fifo, uint32_t _evento_fifo_bits) {
		evento_fifo = _evento_fifo;
		evento_fifo_bits = _evento_fifo_bits;
		gpio_hal_sentido(evento_fifo,1,GPIO_HAL_PIN_DIR_OUTPUT);// se ponen el pin del gpio en un sentido por si ocurre overflow
		gpio_hal_escribir(evento_fifo,evento_fifo_bits,0);
    front = 0;
    rear = 0;
    totalAtendidos=0;
		eventosEncolados=0;
}
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
	//deshabilitar irqs
		int irq_activada = read_IRQ_bit();
		disable_irq();
    if((eventosEncolados)==0){
			enable_irq();
			return 0;
		}
        *ID_evento = cqueue_arr[front].id;
        *auxData = cqueue_arr[front].aux;
        cqueue_arr[front].procesado = 1;
        front = (front + 1) % MAX;
				eventosEncolados--;
				totalAtendidos++;
				if(irq_activada==0){
					enable_irq();
				}
        return 1;
}

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
		//disable_irq();
		int irq_activada = read_IRQ_bit();
		disable_irq();
		if ((eventosEncolados) > MAX) { // if cola llena
        gpio_hal_escribir(evento_fifo, evento_fifo_bits, 1);
        while (1);
    }
			cqueue_arr[rear].id = ID_evento;
			cqueue_arr[rear].procesado = 0;
			cqueue_arr[rear].aux = auxData;
			rear = (rear + 1) % MAX;
			estadisticasEventos[ID_evento]++;
			eventosEncolados++;
				if(irq_activada==0){
					enable_irq();
				}
    }


uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
     // Devuelve el n?mero total de veces que se ha encolado el evento con el ID especificado
    if (ID_evento == 0) {
				return eventosEncolados+totalAtendidos;
    } else {
        return estadisticasEventos[ID_evento];
    }
}


