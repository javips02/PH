#include <LPC210X.h>                            // LPC21XX Peripheral Registers
#include "fifo.h"
#include "gpio_hal.h"

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    front = -1;
    rear = -1;
    //errorPin = pin_overflow;
    // Inicializa el arreglo de estadísticas en cero
    totalAtendidos=0;
}

uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    if (front == -1) {
        //printf("Cola vacía. No hay eventos para extraer.\n");
        return 0; // Indica que la cola está vacía
    }

    // Sacar dato
    *ID_evento = cqueue_arr[front];
    *auxData = ID_evento->auxData;

    // Actualizar índices
    if (front == rear) {
        front = -1;
        rear = -1;
    } else if (front == MAX - 1) {
        front = 0;
    } else {
        front = front + 1;
    }

    return 1; // Indica éxito, se extrajo un evento
}


void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    if ((front == 0 && rear == MAX - 1) || (front == rear + 1)) {
        //printf("Cola llena. No se pueden encolar más eventos.\n");
				gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
        while(1);
    }

    if (front == -1) {
        front = 0;
        rear = 0;
    } else if (rear == MAX - 1) {
        rear = 0;
    } else {
        rear = rear + 1;
    }

    // Guarda el evento en la cola
    cqueue_arr[rear] = ID_evento;
    cqueue_arr[rear].auxData = auxData; // Almacena el campo auxData en la cola

    // Actualiza las estadísticas del evento
    totalAtendidos++;

    // Verifica 
}

uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
    // Devuelve el número total de veces que se ha encolado el evento con el ID especificado
    if (ID_evento.ID_evento == 0) {
        // Si el ID del evento es 0, devuelve el total de eventos encolados desde el inicio
        uint32_t totalEventos = 0;
        for (int i = 1; i < MAX; i++) {
            totalEventos += estadisticasEventos[i];
        }
        return totalEventos;
    } else {
        return estadisticasEventos[ID_evento.ID_evento];
    }
}


