#include "escribirPorPantalla.h"
#include "fifo.h"

void inicializarPartida(uint8_t *nuevoVector){
			int index =0;
		const char *cadenaOriginal = "Bienvenido al juego Conecta 4";
		nuevoVector[index++]='\n';
		while (*cadenaOriginal) {
				nuevoVector[index++] = *cadenaOriginal++;
		}
		const char *cadenaOriginal2 = "Teclea $NEW! para empezar a jugar";
		nuevoVector[index++]='\n';
		while (*cadenaOriginal2) {
				nuevoVector[index++] = *cadenaOriginal2++;
		}	
		nuevoVector[index++]='\n';
		nuevoVector[index++]='\0';
		linea_serie_drv_enviar_array(nuevoVector);
}
void intToStr(uint32_t timeToVisualice,  uint8_t numToStr[15]){ //devuelve en numToStr el numero timeToVisualize en formato char para que se visualice correctamente en la uart
	int i = 0;
    do {
        numToStr[i] = timeToVisualice % 10 + '0'; //Sumamos '0' para convertirlo a caracter
                i++;
        timeToVisualice /= 10;
    } while (timeToVisualice > 0);

    // Le damos la cuelta al string para tener las cifras ordenadas
    int start = 0;
    int end = i - 1;
    while (start < end) {
        // Swap characters
        char temp = numToStr[start];
        numToStr[start] = numToStr[end];
        numToStr[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
    // Null-terminate the string
        numToStr[i++] = ' ';
        numToStr[i++] = 'u';
        numToStr[i++] = 's';
        numToStr[i++] = '\0';

        //Ya tenemos el string, que lo envie la funcion que nos ha llamado en el orden que le convenga
}
void mensajeGanador(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector){
	int index=0;
	nuevoVector[index++]='\n';
	const char *cadenaOriginal = NULL;
	if (res == 1) {
		cadenaOriginal = "!Felicidades! Has ganado la partida jugador ";
	} else if (res == 0) {
		cadenaOriginal = "Se ha rendido el jugador ";
	}
	while (*cadenaOriginal) {
			nuevoVector[index++] = *cadenaOriginal++;
	}
	nuevoVector[index++]='0'+ultimoJugador;
	nuevoVector[index++]='\n';
		nuevoVector[index]='\0';
	linea_serie_drv_enviar_array(nuevoVector);
}
void intToStrEventos(uint32_t timeToVisualice,  uint8_t numToStr[]){ //devuelve en numToStr el numero timeToVisualize en formato char para que se visualice correctamente en la uart
	int i = 0;
    do {
        numToStr[i] = timeToVisualice % 10 + '0'; //Sumamos '0' para convertirlo a caracter
                i++;
        timeToVisualice /= 10;
    } while (timeToVisualice > 0);

    // Le damos la cuelta al string para tener las cifras ordenadas
    int start = 0;
    int end = i - 1;
    while (start < end) {
        // Swap characters
        char temp = numToStr[start];
        numToStr[start] = numToStr[end];
        numToStr[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
}
void finalizarPartida(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector, uint32_t tiempoTotalPartidaIni, 
											uint32_t tiempoTotalHayLinea, uint32_t tiempoTotalHumano, uint32_t numCallsHayLinea, uint32_t numJugadasHumano){ //finaliza partida y escribe por pantalla los resultados
												
												
	int index=0;
	//Tiempos
	//TTotal
	const char *cadenaTiempototal = NULL;
	cadenaTiempototal = "Tiempo total partida: ";
	while (*cadenaTiempototal) {
		nuevoVector[index++] = *cadenaTiempototal++;
	}
	uint32_t tTotal = clock_get_us() - tiempoTotalPartidaIni;
	uint8_t arraytTotal [15];
	intToStr(tTotal, arraytTotal);
	for (int in = 0 ; in < 15; in++){
		if(arraytTotal[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arraytTotal[in];
	}
	nuevoVector[index++] = '\n';
	
	//tHayLinea (total)
	const char *cadenaTHayLinea = NULL;
	cadenaTHayLinea = "Tiempo total hayLinea: ";
	while (*cadenaTHayLinea) {
		nuevoVector[index++] = *cadenaTHayLinea++;
	}
	uint8_t arrayHayLineaTotal[15];
	intToStr(tiempoTotalHayLinea, arrayHayLineaTotal);
	for (int in = 0 ; in < 15; in++){
		if(arrayHayLineaTotal[in]=='\0'){
			break;
			}
		nuevoVector[index++] = arrayHayLineaTotal[in];
		
	}
	nuevoVector[index++] = '\n';
	
	//tHayLinea (avg)
	const char *cadenaTiempoMHayLinea = NULL;
	cadenaTiempoMHayLinea = "Tiempo total hayLinea: ";
	while (*cadenaTiempoMHayLinea) {
		nuevoVector[index++] = *cadenaTiempoMHayLinea++;
	}
	uint8_t arrayHayLineaMedia[15];
	intToStr(tiempoTotalHayLinea/numCallsHayLinea, arrayHayLineaMedia);
	for (int in = 0 ; in < 15; in++){
		if(arrayHayLineaMedia[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayHayLineaMedia[in];
	}
	nuevoVector[index++] = '\n';
	
	//tHumano (total)
	const char *cadenaTiempoTHumano = NULL;
	cadenaTiempoTHumano = "Tiempo total jugador pensando: ";
	while (*cadenaTiempoTHumano) {
		nuevoVector[index++] = *cadenaTiempoTHumano++;
	}
	uint8_t arrayTHumanoTotal[15];
	intToStr(tiempoTotalHumano, arrayTHumanoTotal);
	for (int in = 0 ; in < 15; in++){
		if(arrayTHumanoTotal[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayTHumanoTotal[in];
	}
	nuevoVector[index++] = '\n';
	
	//tHumano (avg)
	const char *cadenaTiempoMHumano = NULL;
	cadenaTiempoMHumano = "Tiempo medio jugador pensando (por jugada): ";
	while (*cadenaTiempoMHumano) {
		nuevoVector[index++] = *cadenaTiempoMHumano++;
	}
	uint8_t arrayTHumanoMedia[15];
	intToStr(tiempoTotalHumano/numJugadasHumano, arrayTHumanoMedia);
	for (int in = 0 ; in < 15; in++){
		if(arrayTHumanoMedia[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayTHumanoMedia[in];
	}
	nuevoVector[index++] = '\n';
	uint8_t arrayEventosTimer[15];
	intToStrEventos(FIFO_estadisticas(TIMER1), arrayEventosTimer);
	const char *evento = NULL;
	evento = "Total de eventos Timer1: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosTimer[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosTimer[in];
	}
	
	nuevoVector[index++]='\n';
	uint8_t arrayEventosEint[15];
	intToStrEventos(FIFO_estadisticas(EINT1), arrayEventosEint);
		evento = "Total de eventos Eint1: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
		for (int in = 0 ; in < 15; in++){
		if(arrayEventosEint[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosEint[in];
	}

	nuevoVector[index++]='\n';	
	uint8_t arrayEventosEint2[15];
	intToStrEventos(FIFO_estadisticas(EINT2), arrayEventosEint2);
	evento = "Total de eventos Eint2: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosEint2[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosEint2[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosPower[15];
	intToStrEventos(FIFO_estadisticas(POWER_DOWN), arrayEventosPower);
	evento = "Total de eventos Powerdown: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosPower[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosPower[in];
	}
	nuevoVector[index++]='\n';

	uint8_t arrayEventosEvLatido[15];
	intToStrEventos(FIFO_estadisticas(ev_LATIDO), arrayEventosEvLatido);
	evento = "Total de eventos ev_LATIDO: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosEvLatido[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosEvLatido[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosev_VISUALIZAR_HELLO[15];
	intToStrEventos(FIFO_estadisticas(ev_VISUALIZAR_HELLO), arrayEventosev_VISUALIZAR_HELLO);
	evento = "Total de eventos ev_VISUALIZAR_HELLO: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosev_VISUALIZAR_HELLO[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosev_VISUALIZAR_HELLO[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosALARMA_OVERFLOW[15];
	intToStrEventos(FIFO_estadisticas(ALARMA_OVERFLOW), arrayEventosALARMA_OVERFLOW);
	evento = "Total de eventos ALARMA_OVERFLOW: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosALARMA_OVERFLOW[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosALARMA_OVERFLOW[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosBOTON[15];
	intToStrEventos(FIFO_estadisticas(BOTON), arrayEventosBOTON);
	evento = "Total de eventos BOTON: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosBOTON[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosBOTON[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosBOTON_TEMPORIZADOR[15];
	intToStrEventos(FIFO_estadisticas(BOTON_TEMPORIZADOR), arrayEventosBOTON_TEMPORIZADOR);
	evento = "Total de eventos BOTON_TEMPORIZADOR: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosBOTON_TEMPORIZADOR[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosBOTON_TEMPORIZADOR[in];
	}
	nuevoVector[index++]='\n';
	uint8_t arrayEventosev_RX_SERIE[15];
	intToStrEventos(FIFO_estadisticas(ev_RX_SERIE), arrayEventosev_RX_SERIE);
	evento = "Total de eventos ev_RX_SERIE: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosev_RX_SERIE[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosev_RX_SERIE[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosev_TX_SERIE[15];
	intToStrEventos(FIFO_estadisticas(ev_TX_SERIE), arrayEventosev_TX_SERIE);
	evento = "Total de eventos ev_TX_SERIE: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosev_TX_SERIE[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosev_TX_SERIE[in];
	}
	nuevoVector[index++]='\n';
	
	uint8_t arrayEventosev_confirmarJugada[15];
	intToStrEventos(FIFO_estadisticas(ev_confirmarJugada), arrayEventosev_confirmarJugada);
	evento = "Total de eventos ev_confirmarJugada: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventosev_confirmarJugada[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventosev_confirmarJugada[in];
	}
		nuevoVector[index++]='\n';
	
	uint8_t arrayEventoseventosTotales[15];
	intToStrEventos(FIFO_estadisticas(IDVOID), arrayEventoseventosTotales);
	evento = "Total de eventos encolados: ";
	while (*evento) {
		nuevoVector[index++] = *evento++;
	}
	for (int in = 0 ; in < 15; in++){
		if(arrayEventoseventosTotales[in]=='\0'){
			break;
		}
		nuevoVector[index++] = arrayEventoseventosTotales[in];
	}
	nuevoVector[index++]='\n';
	
	
	//Nueva partida
	const char *cadenaOriginal2 = NULL;
		cadenaOriginal2 = "Puedes volver a pulsar $NEW! para empezar";
	while (*cadenaOriginal2) {
			nuevoVector[index++] = *cadenaOriginal2++;
	}
	nuevoVector[index++]='\n';
	nuevoVector[index]='\0';
	linea_serie_drv_enviar_array(nuevoVector);
}
											
void jugadaNoValida(uint8_t *nuevoVector){ 
		int index=0;
		nuevoVector[index++]='\n';
			const char *cadenaOriginal = NULL;
			cadenaOriginal = "Columna o fila novalida \n";
		while (*cadenaOriginal) {
				nuevoVector[index++] = *cadenaOriginal++;
		}
		nuevoVector[index]='\0';
		linea_serie_drv_enviar_array(nuevoVector);
}

void comandoNoValido(uint8_t *nuevoVector){
		int index=0;
		nuevoVector[index++]='\n';
			const char *cadenaOriginal = NULL;
			cadenaOriginal = "Comando erroneo \n";
		while (*cadenaOriginal) {
				nuevoVector[index++] = *cadenaOriginal++;
		}
		nuevoVector[index]='\0';
		linea_serie_drv_enviar_array(nuevoVector);
}
void movimientoCancelado(uint8_t *nuevoVector){
		int index=0;
		nuevoVector[index++]='\n';
			const char *cadenaOriginal = NULL;
			cadenaOriginal = "Movimiento Cancelado ";
		while (*cadenaOriginal) {
				nuevoVector[index++] = *cadenaOriginal++;
		}
		nuevoVector[index++]='\n';
		nuevoVector[index++]='\0';
		linea_serie_drv_enviar_array(nuevoVector);
}

void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t, uint8_t *nuevoVector, uint8_t previewMode, uint8_t jugadaActual[], uint8_t jugadorActual){ //hay que mover estas 2 funciones almodulo conecta_K? --> Preguntar en clase
				int index =0;
				nuevoVector[index++]='\n';
				const char *cadenaOriginal2 = "Tablero Actual";
					nuevoVector[index++]='\n';
					while (*cadenaOriginal2) {
							nuevoVector[index++] = *cadenaOriginal2++;
					}
					nuevoVector[index++]='\n';
				for (size_t i =0; i < 8 ; i++){
					for (size_t j = 0; j < 8; j++) {
						//casos a cubrir: tengo i y j al entrar aqui
						//esquina superior izda caso especial
						if (i==0 && j==0) {
							nuevoVector[index++] = '-';
							nuevoVector[index++] = '|';
						}
						//columna 0: F + numI
						else if (i!=0 && j==0){
							nuevoVector[index++] = '0' + i;
							nuevoVector[index++] = '|';
						}
						//fila 0: C + numJ
						else if (i==0 && j!=0) {
							nuevoVector[index++] = '0' + j;
							nuevoVector[index++] = '|';
						}
						//resto de casos: leer tablero y poner info
						else {
							CELDA escribir = tablero_leer_celda(t,i-1,j-1);
							uint8_t c = celda_color(escribir);
							if (previewMode == 0 && i == jugadaActual[0] && j == jugadaActual[1]) { //Poner previsualizacion jugada
								nuevoVector[index++] = 'X'; //caracter de preview de jugada
								nuevoVector[index++] = '|';
							} else if (c == 0){
								nuevoVector[index++] = ' ';
								nuevoVector[index++] = '|';
							} else if (c==1){
								nuevoVector[index++] = 'N';
								nuevoVector[index++] = '|';
							} else {
								nuevoVector[index++] = 'B';
								nuevoVector[index++] = '|';
							}
						}
					}
					nuevoVector[index++] = '\n';
				}
				nuevoVector[index++]='\n';
				const char *cadenaOriginal = NULL;
				if (previewMode == 0){
					cadenaOriginal = "Confirmar Jugada?(Tienes 3 segundos para confirmar) \n EINT1 para cancelar jugada, Jugador Actual: ";
					nuevoVector[index++] = jugadaActual[0];
					nuevoVector[index++] = '-';
					nuevoVector[index++] = jugadaActual[1];
				}else if(previewMode==2){
					cadenaOriginal = "Este ha sido el tablero final";
				} 
				else {
					cadenaOriginal = "Le toca al jugador:  ";
				}
				while (*cadenaOriginal) {
						nuevoVector[index++] = *cadenaOriginal++;
				}
				nuevoVector[index++] = '0' + jugadorActual;
				nuevoVector[index++] = '\n';
				nuevoVector[index++]='\0';
				linea_serie_drv_enviar_array(nuevoVector);
}
void partidaEmpatada(uint8_t *nuevoVector){
			int index=0;
			const char *cadenaOriginal = NULL;
			cadenaOriginal="Partida acabada en empate";
			while (*cadenaOriginal) {
					nuevoVector[index++] = *cadenaOriginal++;
			}
			nuevoVector[index++] = '\n';
			nuevoVector[index++]='\0';
			linea_serie_drv_enviar_array(nuevoVector);	
}


