# Cosas correccion pr2c

fifo.h solo desde planificador
hacer modulo eventos_H que puedan incluir todos los modulos
IO_reseva solo lo conoce planificador
Ser muy estrictos con hal y drv. Include del lpc21.0... sólamente en modulos HW
Varibales concurrentes static
Variables que vayan a ser modificadas fuera del flujo normal de ejecucion son Volatile

    - Poner tiempo con unidades en el output V
    - Terminar de arreglasr includes
    - Documentacion: pequeños comentarios en los headers de todos los modulos
    - Volatiles y Statics:
    - Poner en header solamente aquellas cosa que tengan que ser visibles desde fuera
    - Añadir seccion critica (no IRQs) en encolar
    - Implementar power_hal drv par apoder llamar desde planificador

# Apuntes pr3

Main con: planificador

## Modulo juego:
    - Al iniciar, llamar a funcion de welcome que escriba por pantalla reglas y como jugar (similar a la de msotrar tablero)
### Juego tratar evento
    - $NEW --> Cargar tablero de test y mostrar por pantalla el jugador al que le toca jugador
    - $END --> Deja de mostrar por pantalla el tablero y el jugador, y deja de recibir comandos del tipo  $#‐#! (los ignora)
    -  $#‐#! --> Juega: llama a la fucnion de conecta_k que Juega --> LLAMAR A conceta_k_juga,, con 2 paramas (fila y col)
        - Comprobar que  fila < NUMFILS y col < NUMCOLS
        - Hacer jugada
        - Comprobar alineamiento

## Plazo de 3 segundos para cancelar
    - 2 Opciones: funcion de borrar del tablero // funcion de previsualizar
    - Variable "puedeCancelar" en el modulo juego
    - Cuando el usuario juega, se pone a 1 y se activa una alarma
    - Cuando la alarma salta, se encola un evento que hace que la variable se ponga a 0 de nuevo
