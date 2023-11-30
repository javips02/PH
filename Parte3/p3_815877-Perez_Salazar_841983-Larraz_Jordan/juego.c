#include "juego.h"
#define true 0
#define false 1
#include "gestorAlarma.h" //REVISAR SI SE PUEDE HACER INCLUDE, SINO HAY QUE HACER OTRO EVENTO PARA QUE LO HAGA EL PLANIFICADOR (ev_activarAlarma o algo asi)
//Variables
//static uint32_t cuentaJuego, intervalo;
static uint8_t salida[8][8];
static uint8_t nuevoVector[200];
static TABLERO cuadricula;
static uint32_t tIniMostrarTablero;
static uint32_t tFinMostrarTablero;
static uint8_t numToStr[14]; //nums de hasta 10 cifras (uint32_t llega hasta ahi) + unidades y fin de linea y de string
static uint8_t jugadorActual;;
static uint8_t mostrartiempo;
static uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t jugadaActual[2] = {0,0}; //Datos jugada actual 
static uint8_t previewMode; //Nos indica si estamos en los 3 segundos de gracia para confirmar jugada
static void (*encolar)()=NULL;
//Funciones
void juego_inicializar(void (*funcion_encolar_evento)()){
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
		tIniMostrarTablero = 0;
		tFinMostrarTablero = 0;
		jugadorActual=1;
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
		encolar=funcion_encolar_evento;
}

void juego_tratar_evento(EVENTO_T ID_Evento, uint32_t auxData){
		if(ID_Evento == ev_RX_SERIE){
			char letra1 = (char)(auxData & 0xFF);         // Bits 0-7
			char letra2 = (char)((auxData >> 8) & 0xFF);  // Bits 8-15
			char letra3 = (char)((auxData >> 16) & 0xFF); // Bits 16-23
			if(letra1 == 'B' && letra2 == 'A' && letra3=='T'){
				//empezar a contar
				disable_irq();
				tIniMostrarTablero = clock_get_us();
				enable_irq();
				mostrartiempo=1;
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			}else if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){
				disable_irq();
				tIniMostrarTablero = clock_get_us();
				enable_irq();
				mostrartiempo=1;
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			}else if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {
						letra1 = letra1 - '0';
						letra3= letra3 - '0';
						entrada[0]=1;
						entrada[1]=letra1;
						entrada[2]=letra3;
						entrada[3]=jugadorActual;
						mostrartiempo=1;
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula); //aqui no jugamos, solo si nos confirman
						jugadaActual[0] = letra1;
						jugadaActual[1] = letra3;
						previewMode = true;
					} //GESTIONAR ERROR RANGOS	SEGUN INSTRUCCIONES (gpio flag y ns que mas)
				}
		}else if(ID_Evento == ev_TX_SERIE){
				if(mostrartiempo==1){
					//parar de contar
					disable_irq();
					tFinMostrarTablero = clock_get_us();
					enable_irq();
					uint32_t tTotal = tFinMostrarTablero - tIniMostrarTablero;
					
					//mostrar tiempo por pantalla
					conecta_K_visualizar_tiempo(tTotal);
					tIniMostrarTablero = 0;
					tFinMostrarTablero = 0;
					mostrartiempo=0;
				}
		} else if (ID_Evento == EINT1 && previewMode == true) { //evitamos que entre cuando no ha preview
			previewMode = false;
			alarma_activar(ev_confirmarJugada, 0, 0); //cancelamos alarma para dejarla libre
			conecta_K_hacer_jugada(entrada, &cuadricula, salida);
			conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
		} else if (ID_Evento == EINT2) { //Cancelar jugada
			if ( previewMode == true) { //Cancelar movimiento
				previewMode = false;
				conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
			} else { //Jugador se rinde
				//implementyar func que meta el comando $END! 
				encolar(ev_RX_SERIE, '$');
				encolar(ev_RX_SERIE, 'E');
				encolar(ev_RX_SERIE, 'N');
				encolar(ev_RX_SERIE, 'D');
				encolar(ev_RX_SERIE, '!');
			}
		} else if (ID_Evento == ev_confirmarJugada && previewMode == true) { //evitamos que entre cuando no ha preview
			previewMode = false;
			conecta_K_hacer_jugada(entrada, &cuadricula, salida);
			conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
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
							if (previewMode == true && i-1 == jugadaActual[0] && j-1 == jugadaActual[1]) { //Poner previsualizacion jugada
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
					cadenaOriginal = "Confirmar Jugada?(Tienes 3 sgundos para confirmar)\n EINT1 para confirmar, EINT2 para cancelar \n Jugada Actual:"; //Deberiamos mostrar la cuenta atras?
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
				alarma_activar(ev_confirmarJugada, 3000, 0);
}

void conecta_K_visualizar_tiempo(uint32_t timeToVisualice){
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

