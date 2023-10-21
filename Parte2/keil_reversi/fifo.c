#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "fifo.h"
EVENTO cqueue_arr[MAX];
int front = -1;
int rear = -1;
// Variable para mantener un registro de estadísticas
uint32_t estadisticasEventos[MAX];
uint32_t eventosEncolados;
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    front = -1;
    rear = -1;
    //errorPin = pin_overflow;
    // Inicializa el arreglo de estadísticas en cero
    totalAtendidos=0;
		eventosEncolados=0;
}

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
        if (cqueue_arr[front].procesado == 0) {
            // Se ha encontrado un evento no procesado
            *ID_evento = cqueue_arr[totalAtendidos%MAX].id_evento; // Actualizar ID_evento con el valor del evento
            cqueue_arr[front].procesado = 1; // Marcar el evento como procesado
						totalAtendidos++;
						front = (front +1)%MAX;
            return 1; // Devuelve 1 para indicar que se ha extraído un evento con éxito
        }
		return 0;
}

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
		if ((front == 0 && rear == MAX - 1) || (front == rear + 1)) { // if cola llena
        gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
        while (1);
    }
    else if (front == -1) { // if cola vacia
        front = 0;
        rear = 0;
    } else {  // puedo meter en la cola
        rear = (rear + 1) % MAX;
				}
				cqueue_arr[rear].id_evento = ID_evento;
				cqueue_arr[rear].procesado = 0;
				cqueue_arr[rear].stats = auxData;
				estadisticasEventos[ID_evento]++;
				eventosEncolados++;
    }

uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
     // Devuelve el número total de veces que se ha encolado el evento con el ID especificado
    if (ID_evento == 0) {
				return eventosEncolados;
    } else {
        return estadisticasEventos[ID_evento];
    }
}


