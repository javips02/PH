#include "juego.h"
#define true 0
#define false 1
#include "gestorAlarma.h" //REVISAR SI SE PUEDE HACER INCLUDE, SINO HAY QUE HACER OTRO EVENTO PARA QUE LO HAGA EL PLANIFICADOR (ev_activarAlarma o algo asi)
#include "conecta_K_2023.h"
//Variables
//static uint32_t cuentaJuego, intervalo;
static uint8_t salida[8][8];
static uint8_t nuevoVector[10000];
static TABLERO cuadricula;
static uint32_t tIniMostrarTablero;
static uint32_t tFinMostrarTablero;
static uint8_t numToStr[14]; //nums de hasta 10 cifras (uint32_t llega hasta ahi) + unidades y fin de linea y de string
static uint8_t jugadorActual;;
static uint8_t mostrartiempo;
static uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t jugadaActual[2] = {0,0}; //Datos jugada actual 
static uint8_t previewMode; //Nos indica si estamos en los 3 segundos de gracia para confirmar jugada
static uint8_t primeraJugada;
static uint8_t primeraPartida;
static uint8_t numeroJugadaActual;
static uint32_t evento_juego_error, evento_juego_error_bits;
static uint8_t estado_error;
static uint8_t finalizar;
static uint8_t jugadorAnterior;
static uint32_t tiempoTotalPartidaIni; //Estadistica de tiempo sin contar el pwowerDown 
static uint32_t tiempoTotalHayLinea;
static uint32_t numCallsHayLinea;
static uint32_t tiempoTotalHumano;
static uint32_t numJugadasHumano;
uint32_t tIniMovimientoHumano;
typedef enum {
    VICTORIA,
    RENDICION
} ResultadoPartida;

//Funciones auxiliares
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
		numToStr[i++] = '\n';
		
		//Ya tenemos el string, que lo envie la funcion que nos ha llamado en el orden que le convenga
}

void finalizarPartida(ResultadoPartida res, uint8_t ultimoJugador){ //finaliza partida y escribe por pantalla los resultados
	int index=0;
	nuevoVector[index++]='\n';
	const char *cadenaOriginal = NULL;
	if (res == VICTORIA) {
		cadenaOriginal = "!Felicidades! Has ganado la partida jugador ";
	} else if (res == RENDICION) {
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
			continue;
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
			continue;
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
			continue;
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
			continue;
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
			continue;
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


void juego_inicializar(uint32_t _evento_juego_error, uint32_t _evento_juego_error_bits){ 
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
		evento_juego_error = _evento_juego_error;
		evento_juego_error_bits=_evento_juego_error_bits;
		gpio_hal_sentido(evento_juego_error, evento_juego_error_bits, GPIO_HAL_PIN_DIR_OUTPUT);
		gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
		tIniMostrarTablero = 0;
		tFinMostrarTablero = 0;
		jugadorAnterior=0;
		finalizar=0;
		primeraPartida=1;
		jugadorActual=1;
		numeroJugadaActual=0;
		estado_error=0;
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
		//hello();
		jugadaActual [0] = 0;
		jugadaActual [1] = 0;
		previewMode = false;
		primeraJugada=1;
		tiempoTotalPartidaIni = clock_get_us();
		tiempoTotalHayLinea = 0;
		numCallsHayLinea = 0;
		tiempoTotalHumano = 0;
		numJugadasHumano = 0;
		tIniMovimientoHumano = clock_get_us();
}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){ //trata los eventos provenientes del planificador relacionados con el modulo juego
		if(ID_Evento == ev_RX_SERIE){
			char letra1 = (char)(auxData & 0xFF);         // Bits 0-7
			char letra2 = (char)((auxData >> 8) & 0xFF);  // Bits 8-15
			char letra3 = (char)((auxData >> 16) & 0xFF); // Bits 16-23
		 if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){
			 if(estado_error==1){
				estado_error=0;
				gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
			 }
			 
			 if(primeraPartida==1){
				 primeraPartida=0;
				 disable_irq();
				tIniMostrarTablero = clock_get_us();
				enable_irq();
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			 }else{
				 jugadaActual [0] = 0;
				jugadaActual [1] = 0;
				previewMode = false;
				primeraJugada=1;
				jugadorActual=1;
				numeroJugadaActual=0;
				disable_irq();
				tIniMostrarTablero = clock_get_us();
				enable_irq();
				 tablero_inicializar(&cuadricula);
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			 }
				
			}else if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {
						if(estado_error==1){
							estado_error=0;
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
						 }
						letra1 = letra1 - '0';
						letra3= letra3 - '0';
						entrada[0]=1;
						entrada[1]=letra3;
						entrada[2]=letra1;
						entrada[3]=jugadorActual;
						jugadaActual[0] = letra3;
						jugadaActual[1] = letra1;
						if(conecta_K_intentar_jugada(entrada, &cuadricula, salida) == 1){
							previewMode = true;
							/*
							disable_irq();
							tIniMostrarTablero = clock_get_us();
							enable_irq();*/
							alarma_activar(ev_confirmarJugada, 1000, 0);
							conecta_K_visualizar_tablero(&cuadricula, salida);
							conecta_K_visualizar_tablero_por_pantalla(&cuadricula); //aqui no jugamos, solo si nos confirman
						}else{
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
							estado_error=1;
							jugadaNoValida();
						}	
					}else{
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
						estado_error=1;
						jugadaNoValida();
						//enviar Fila o columna no valida
					} //GESTIONAR ERROR RANGOS	SEGUN INSTRUCCIONES (gpio flag y ns que mas)
				}else{
					gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
					estado_error=1;
					comandoNoValido();
				}
		}else if(ID_Evento == ev_TX_SERIE){
			if(finalizar==1){
				finalizar =0;
				finalizarPartida(VICTORIA, jugadorAnterior);
			}
		} else if (ID_Evento == ev_confirmarJugada && previewMode == true) { //evitamos que entre cuando no ha preview
			tiempoTotalHumano += clock_get_us() - tIniMovimientoHumano;		//sumamos el tiempo de jugador 	
			numJugadasHumano++;
			previewMode = false;
				entrada[0]=1;
				entrada[1]=jugadaActual[0];
				entrada[2]=jugadaActual[1];
				entrada[3]=jugadorActual;
				conecta_K_hacer_jugada(entrada, &cuadricula, salida);
				jugadorAnterior=jugadorActual;
				if(primeraJugada==1){
							jugadorActual = ((jugadorActual%2)+1);
							primeraJugada=0;
				}else{
					numeroJugadaActual++;
					if(numeroJugadaActual==2){
						jugadorActual = ((jugadorActual%2)+1);
						numeroJugadaActual=0;
					}
				}
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
				uint32_t tini = clock_get_us();
				uint32_t hayLinea= conecta_K_verificar_K_en_linea(&cuadricula, jugadaActual[0]-1, jugadaActual[1]-1, jugadorAnterior);				
				tiempoTotalHayLinea += ( clock_get_us() - tini); //Estadisticas de HayLinea
				numCallsHayLinea++;
				if (hayLinea == 0){
					//while(1);
				}else{
					// while(1);
					finalizar=1;
					
				}
				tIniMovimientoHumano = clock_get_us(); //TENGO DUDAS, NO SE SI EMPEZAR AQUI EL TIEMPO DE USUARIO DE NUEVO
		} else if (auxData == EINT1) { //Cancelar jugada
			if ( previewMode == true) { //Cancelar movimiento
				previewMode = false;
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
				alarma_activar(ev_confirmarJugada, 0, 0);
				//conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			}
		}else if(auxData == EINT2){
			alarma_activar(ev_confirmarJugada, 0, 0);
				finalizarPartida(RENDICION,jugadorActual);
		}
}
void conecta_K_visualizar_tablero_por_pantalla(TABLERO *t){ //hay que mover estas 2 funciones almodulo conecta_K? --> Preguntar en clase
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
							if (previewMode == true && i == jugadaActual[0] && j == jugadaActual[1]) { //Poner previsualizacion jugada
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
				if (previewMode == true){
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

void conecta_K_visualizar_tiempo(uint32_t timeToVisualice){ //transforma timeToVisualize a formato printeable en la uart y lo envia a linea_serie
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
		numToStr[i++] = '\n';
    numToStr[i++] = '\0';
		
		//Ya tenemos el string, ahora lo mandamos
		linea_serie_drv_enviar_array(numToStr);
}

void jugadaNoValida(void){ 
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

void comandoNoValido(void){
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

