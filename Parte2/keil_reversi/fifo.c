#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "fifo.h"
EVENTO cqueue_arr[MAX];
int front = -1;
int rear = -1;
// Variable para mantener un registro de estadísticas
uint32_t estadisticasEventos[MAX];
uint32_t eventosEncolados;
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    front = 0;
    rear = 0;
    totalAtendidos=0;
		eventosEncolados=0;
}
/*
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
				if(front==-1){return 0;}
        if (cqueue_arr[front].procesado == 0) {
            // Se ha encontrado un evento no procesado
            ID_evento = &cqueue_arr[front].id; // Actualiza el contenido de ID_evento
						auxData = &cqueue_arr[front].aux;
            cqueue_arr[front].procesado = 1; // Marcar el evento como procesado
						totalAtendidos++;
						front = (front +1)%MAX;
            return 1; // Devuelve 1 para indicar que se ha extraído un evento con éxito
        }
		return 0;
}*/
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    if((eventosEncolados)==0){
			power_hal_wait();
			return 0;
		}
        ID_evento = &cqueue_arr[front].id;
        auxData = &cqueue_arr[front].aux;
        cqueue_arr[front].procesado = 1;
        front = (front + 1) % MAX;
				eventosEncolados--;
        return 1;
}

void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
		if ((eventosEncolados % MAX) > 0) { // if cola llena
        gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
        while (1);
    }
      rear = (rear + 1) % MAX;
			cqueue_arr[rear].id = ID_evento;
			cqueue_arr[rear].procesado = 0;
			cqueue_arr[rear].aux = auxData;
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


