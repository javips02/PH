#include "juego.h"
#define true 0
#define false 1
#include "gestorAlarma.h" //REVISAR SI SE PUEDE HACER INCLUDE, SINO HAY QUE HACER OTRO EVENTO PARA QUE LO HAGA EL PLANIFICADOR (ev_activarAlarma o algo asi)
#include "conecta_K_2023.h"
//Variables
static uint8_t salida[8][8]; 
static uint8_t nuevoVector[1000];
static TABLERO cuadricula;
static uint8_t jugadorActual;
static uint8_t primerJugador;
static uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t jugadaActual[2] = {0,0}; //Datos jugada actual 
static uint8_t previewMode; //Nos indica si estamos en los 3 segundos de gracia para confirmar jugada
static uint8_t primeraJugada;
static uint8_t primeraPartida;
static uint8_t numeroJugadaActual;
static uint32_t evento_juego_error, evento_juego_error_bits;
static uint8_t jugadorAnterior;
static uint32_t tiempoTotalPartidaIni; //Estadistica de tiempo sin contar el pwowerDown 
static uint32_t tiempoTotalHayLinea;
static uint32_t numCallsHayLinea;
static uint32_t tiempoTotalHumano;
static uint32_t numJugadasHumano;
static uint32_t tIniMovimientoHumano;
typedef enum {
		IDLE, //ev_RXSERIE (Lo ponemos desde ini y al acabar partida por rendicion o por victoria)
		MSG_ERROR, //ev_RXSERIE
		NEW_GAME, 
		FIRST_GAME,
		ESPERAR_JUGADA, //ev_RXSERIE
		ERROR_CMD, //ev_RXSERIE
		PREVIEW_MOVE, 
		POWERDOWN, 
		EXEC_MOVE,
		ESTADISTICAS,
    VICTORIA,
    RENDICION
} Estado;
static Estado estadoActual=IDLE;

//Funciones auxiliares
void juego_inicializar(uint32_t _evento_juego_error, uint32_t _evento_juego_error_bits){ 

		evento_juego_error = _evento_juego_error;
		evento_juego_error_bits=_evento_juego_error_bits;
		gpio_hal_sentido(evento_juego_error, evento_juego_error_bits, GPIO_HAL_PIN_DIR_OUTPUT);
		gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
		jugadorAnterior=0;
		primerJugador=1;
		primeraPartida=1;
		jugadorActual=1;
		numeroJugadaActual=0;
		estadoActual=IDLE;
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
void juego_ini_primera(void){
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
		jugadorAnterior=0;
		primerJugador=1;
		jugadorActual=1;
		numeroJugadaActual=0;
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
};
void juego_ini(void){
		tablero_inicializar(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
		jugadorAnterior=0;
		primerJugador=(primerJugador%2)+1;
		jugadorActual=primerJugador;
		numeroJugadaActual=0;
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
			//Coger letras del evento
			char letra1 = (char)(auxData & 0xFF);         // Bits 0-7
			char letra2 = (char)((auxData >> 8) & 0xFF);  // Bits 8-15
			char letra3 = (char)((auxData >> 16) & 0xFF); // Bits 16-23
			if(estadoActual == IDLE){ //ESTADO IDLE
				if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){
					if(primeraPartida==1){
						estadoActual = FIRST_GAME; //nos movemos a first game y ejecutamos directamente sus acciones (transicion automatica unica)
						primeraPartida=0;
						juego_ini_primera();
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
						estadoActual=ESPERAR_JUGADA; //nos movemos a esperar jugada (ejecutaremos cuando se disparen las condiciones)
					}else{
						estadoActual=NEW_GAME; //nos movemos a new game y ejecutamos directamente sus acciones (transicion automatica unica)
						juego_ini();
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
						estadoActual=ESPERAR_JUGADA; //nos movemos a esperar jugada (ejecutaremos cuando se disparen las condiciones)
					}
				}else{
					estadoActual=MSG_ERROR;
					comandoNoValido(nuevoVector);
				}
			}
			else if(estadoActual==MSG_ERROR){ //ESTADO MSG_ERROR (estaba en idle y el comando es incorrecto)
				if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){
					if(primeraPartida==1){
						primeraPartida=0;
						estadoActual=ESPERAR_JUGADA;
						juego_ini_primera();
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
					}else{
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
						juego_ini();
						estadoActual=ESPERAR_JUGADA;
					}
				}else if(letra1 == 'D' && letra2 == 'N' && letra3=='E'){
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorActual, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
				}else{
					comandoNoValido(nuevoVector);
					estadoActual=MSG_ERROR;
				}	
			}
			else if(estadoActual==ESPERAR_JUGADA){ //ESTADO ESPERAR_JUGADA 
				if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {
						/*if(estado_error==1){
							estado_error=0;
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
						 }*/
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
							alarma_activar(ev_confirmarJugada, 1000, 0);
							conecta_K_visualizar_tablero(&cuadricula, salida);
							conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual); //aqui no jugamos, solo si nos confirman
							estadoActual=PREVIEW_MOVE;
						}else{
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
							//estado_error=1;
							jugadaNoValida(nuevoVector);
							estadoActual=ERROR_CMD;
						}	
					}else{
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
						//estado_error=1;
						estadoActual=ERROR_CMD;
						jugadaNoValida(nuevoVector);
					} //GESTIONAR ERROR RANGOS	SEGUN INSTRUCCIONES (gpio flag y ns que mas)
				}else if(letra1 == 'D' && letra2 == 'N' && letra3=='E'){
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorActual, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
				}else{
					gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
					//estado_error=1;
					estadoActual=ERROR_CMD;
					comandoNoValido(nuevoVector);
				}
			}	
			else if (estadoActual == ERROR_CMD) {
				if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {
						/*if(estado_error==1){
							estado_error=0;
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
						 }*/
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
							alarma_activar(ev_confirmarJugada, 1000, 0);
							conecta_K_visualizar_tablero(&cuadricula, salida);
							conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual); //aqui no jugamos, solo si nos confirman
							estadoActual=PREVIEW_MOVE;
						}else{
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
							//estado_error=1;
							jugadaNoValida(nuevoVector);
							estadoActual=ERROR_CMD;
						}	
					}else{
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
						//estado_error=1;
						estadoActual=ERROR_CMD;
						jugadaNoValida(nuevoVector);
					} //GESTIONAR ERROR RANGOS	SEGUN INSTRUCCIONES (gpio flag y ns que mas)
				}else if(letra1 == 'D' && letra2 == 'N' && letra3=='E'){
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorActual, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
				}else{
					gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
					//estado_error=1;
					estadoActual=ERROR_CMD;
					comandoNoValido(nuevoVector);
				}
			}
	}else if(estadoActual==PREVIEW_MOVE){
					if (ID_Evento == ev_confirmarJugada) { //evitamos que entre cuando no ha preview
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
							conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
							
							estadoActual=EXEC_MOVE;
							
					}else if (auxData == EINT1) { //Cancelar jugada
							//if ( previewMode == true) { //Cancelar movimiento
								//previewMode = false;
								
								alarma_activar(ev_confirmarJugada, 0, 0);
								estadoActual=ESPERAR_JUGADA;
								movimientoCancelado(nuevoVector);
								//conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
							}
					}else if(estadoActual==EXEC_MOVE){
								uint32_t tini = clock_get_us();
								uint32_t hayLinea= conecta_K_verificar_K_en_linea(&cuadricula, jugadaActual[0]-1, jugadaActual[1]-1, jugadorAnterior);				
								tiempoTotalHayLinea += ( clock_get_us() - tini); //Estadisticas de HayLinea
								numCallsHayLinea++;
								if (hayLinea == 0){
									estadoActual=ESPERAR_JUGADA;
									//while(1);
								}else{
									// while(1);
									estadoActual = VICTORIA;
									conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
								}
								tIniMovimientoHumano = clock_get_us(); //TENGO DUDAS, NO SE SI EMPEZAR AQUI EL TIEMPO DE USUARIO DE NUEVO
					} else if (estadoActual == VICTORIA) {
						if (ID_Evento == ev_TX_SERIE) {
								finalizarPartida(1,jugadorAnterior, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
								estadoActual=IDLE;
						}
					}
					else if (estadoActual == PREVIEW_MOVE || estadoActual == ESPERAR_JUGADA || estadoActual == EXEC_MOVE || estadoActual==ERROR_CMD){
						if(auxData == EINT2){
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorAnterior, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
						}
			}
}
