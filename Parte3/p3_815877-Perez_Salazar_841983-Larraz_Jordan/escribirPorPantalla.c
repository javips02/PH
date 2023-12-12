#include "escribirPorPantalla.h"

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

void finalizarPartida(uint8_t res, uint8_t ultimoJugador, uint8_t *nuevoVector, uint32_t tiempoTotalPartidaIni, 
											uint32_t tiempoTotalHayLinea, uint32_t tiempoTotalHumano, uint32_t numCallsHayLinea, uint32_t numJugadasHumano){ //finaliza partida y escribe por pantalla los resultados
												
												
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
			cadenaOriginal = "Comando no valido \n";
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
					cadenaOriginal = "Confirmar Jugada?(Tienes 3 sgundos para confirmar) \n EINT1 para cancelar jugada, Jugada Actual: "; //Deberiamos mostrar la cuenta atras?
					nuevoVector[index++] = jugadaActual[0];
					nuevoVector[index++] = '-';
					nuevoVector[index++] = jugadaActual[1];
				} else {
					cadenaOriginal = "Jugador actual = ";
				}
				while (*cadenaOriginal) {
						nuevoVector[index++] = *cadenaOriginal++;
				}
				nuevoVector[index++] = '0' + jugadorActual;
				nuevoVector[index++] = '\n';
				nuevoVector[index++]='\0';
				linea_serie_drv_enviar_array(nuevoVector);
}

