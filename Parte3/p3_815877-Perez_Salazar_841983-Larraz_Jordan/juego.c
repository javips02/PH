//Fichero de implementacion de juego.h
#include "juego.h"
#define true 0
#define false 1
#include "gestorAlarma.h" 
#include "conecta_K_2023.h"
#include "entrada.h"
#include "escribirPorPantalla.h"

//Variables que serviran para inicializar el tablero a usar, el buffer que se enviara a linea de serie, la salida por memoria del tablero y la entrada de las jugadas
static uint8_t salida[8][8]; 
static uint8_t nuevoVector[1000];
static TABLERO cuadricula;
static uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 };


static uint8_t jugadorActual;//variable para el jugador actual
static uint8_t primerJugador;//variable para saber quien tiene que empezar en cada partida
static uint8_t jugadaActual[2] = {0,0}; //variable auxiliar para saber quien juega en cada momento
static uint8_t primeraJugada;//variable para saber si es la primera jugada de la partida 
static uint8_t primeraPartida;// variable para saber si es la primera partida  y hay que cargar el tablero de test. 
static uint8_t numeroJugadaActual;// variable para saber cuantas jugadas quedan en cada turno
static uint8_t jugadorAnterior; // variable que sabra quien ha ganado o rendido

static uint8_t previewMode; //Nos indica si estamos en los 3 segundos de gracia para confirmar jugada


static uint32_t evento_juego_error, evento_juego_error_bits;// necesario para actualizar el gpio ya que no savemos que evento somos ni los bits

//variables para contar el tiempo de ejecucion en distintas fases
static uint32_t tiempoTotalPartidaIni; //Estadistica de tiempo sin contar el pwowerDown 
static uint32_t tiempoTotalHayLinea;// tiempo en la fujncion de haylinea
static uint32_t numCallsHayLinea;//variable para sacar la media con el numeor de veces que se llama a HayLinea
static uint32_t tiempoTotalHumano;
static uint32_t numJugadasHumano;
static uint32_t tIniMovimientoHumano;

//estados de la maquina de estados posibles
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
    RENDICION,
		EMPATE
} Estado;
static Estado estadoActual=IDLE;

//Funciones auxiliares
//inicializa el bit gpio y las estructuras de datos
void juego_inicializar(uint32_t _evento_juego_error, uint32_t _evento_juego_error_bits){ 

		evento_juego_error = _evento_juego_error;
		evento_juego_error_bits=_evento_juego_error_bits;
		gpio_hal_sentido(evento_juego_error, evento_juego_error_bits, GPIO_HAL_PIN_DIR_OUTPUT);
		gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
		jugadorAnterior=1;
		primerJugador=1;
		primeraPartida=1;
		jugadorActual=1;
		numeroJugadaActual=0;
		estadoActual=IDLE;
		inicializarPartida(nuevoVector);
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
//inicializa el tablero cargando el tablero test
void juego_ini_primera(void){
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);
};
//incializa el tablero a cero y los datos necesarios para la ejecucion
void juego_ini(void){
		tablero_inicializar(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);

		primerJugador=(primerJugador%2)+1;// se cambia el turno
		jugadorActual=primerJugador;
		numeroJugadaActual=0;
		jugadorAnterior=jugadorActual;
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
		if(ID_Evento == ev_RX_SERIE){// hay estados que solo aceptan trasiciones con eventos especificos
			//Coger letras del evento
			char letra1 = (char)(auxData & 0xFF);         // Bits 0-7
			char letra2 = (char)((auxData >> 8) & 0xFF);  // Bits 8-15
			char letra3 = (char)((auxData >> 16) & 0xFF); // Bits 16-23
			if(estadoActual == IDLE){ //ESTADO IDLE que solo aceptara $NEW! o si es no valido ira a otro estado de error
				if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){
					if(primeraPartida==1){//para saber si se esta creando una partida que es la primeera o no
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
					gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
					comandoNoValido(nuevoVector);
				}
			}
			else if(estadoActual==MSG_ERROR){ //ESTADO MSG_ERROR (estaba en idle y el comando es incorrecto)
				if(letra1 == 'W' && letra2 == 'E' && letra3=='N'){//si me llega new esperare un comando en otro estado
					if(primeraPartida==1){
						primeraPartida=0;
						estadoActual=ESPERAR_JUGADA;
						juego_ini_primera();
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
					}else{
						juego_ini();
						conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
						estadoActual=ESPERAR_JUGADA;
					}
				}/*else if(letra1 == 'D' && letra2 == 'N' && letra3=='E'){
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorActual, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
				}*/else{
					comandoNoValido(nuevoVector);
					estadoActual=MSG_ERROR;
				}	
			}
			else if(estadoActual==ESPERAR_JUGADA){ //ESTADO ESPERAR_JUGADA 
				if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {// si el comnado es valido se intentara la jugada sino se ira a un estado de error
						letra1 = letra1 - '0';
						letra3= letra3 - '0';
						entrada[0]=1;
						entrada[1]=letra3;
						entrada[2]=letra1;
						entrada[3]=jugadorActual;
						jugadaActual[0] = letra3;
						jugadaActual[1] = letra1;
						if(conecta_K_intentar_jugada(entrada, &cuadricula, salida) == 1){// su se puede hacer la jugada se activa la alarma sino es posible el movimiento se va a un estado de error
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
						estadoActual=ERROR_CMD;
						jugadaNoValida(nuevoVector);
					} //GESTIONAR ERROR RANGOS	SEGUN INSTRUCCIONES (gpio flag y ns que mas)
				}else if(letra1 == 'D' && letra2 == 'N' && letra3=='E'){// si me llega un $END! se acaba la partida
							alarma_activar(ev_confirmarJugada, 0, 0);
							finalizarPartida(0,jugadorActual, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
							estadoActual = IDLE;
				}else{
					gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 1);
					estadoActual=ERROR_CMD;
					comandoNoValido(nuevoVector);
				}
			}	
			else if (estadoActual == ERROR_CMD) {//estado parecido al anterior q solo abandona si le llega una jugada correcta
				if(letra2=='-'){
				  if (letra1 >= '1' && letra1 <= '7' && letra3 >='1' && letra3<='7') {
						gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);
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
							gpio_hal_escribir(evento_juego_error, evento_juego_error_bits, 0);					
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
	}else if(estadoActual==PREVIEW_MOVE){//estado en el que se espera 3 segundos y llegara el evento de confirmar o un boton 
							if (ID_Evento == ev_confirmarJugada) { // si me llega la alarma se hara la jugada 
								tiempoTotalHumano += clock_get_us() - tIniMovimientoHumano;		//sumamos el tiempo de jugador 	
								numJugadasHumano++;
								previewMode = false;
								entrada[0]=1;
								entrada[1]=jugadaActual[0];
								entrada[2]=jugadaActual[1];
								entrada[3]=jugadorActual;
								if(conecta_K_hacer_jugada(entrada, &cuadricula, salida)==0){
										conecta_K_visualizar_tablero_por_pantalla(&cuadricula, nuevoVector, previewMode, jugadaActual, jugadorActual);
										estadoActual=EMPATE;
								}else{
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
								}			
							}else if (auxData == EINT1){ //Cancelar jugada y permanecera en el mismo estado
										alarma_activar(ev_confirmarJugada, 0, 0);
										estadoActual=ESPERAR_JUGADA;
										movimientoCancelado(nuevoVector);
										//conecta_K_visualizar_tablero_por_pantalla(&cuadricula);
									}
					}else if(estadoActual==EXEC_MOVE){// estado en el que se ejecuta la jugada y ve si ha ganado o no. Si se gana se imprimira el tablero ganador
								uint32_t tini = clock_get_us();
								uint32_t hayLinea= conecta_K_verificar_K_en_linea(&cuadricula, jugadaActual[0]-1, jugadaActual[1]-1, jugadorAnterior);				
								tiempoTotalHayLinea += ( clock_get_us() - tini); //Estadisticas de HayLinea
								numCallsHayLinea++;
								if (hayLinea == 0){
									estadoActual=ESPERAR_JUGADA;
								}else{
									estadoActual = VICTORIA;
									mensajeGanador(1, jugadorAnterior, nuevoVector);
								}
								tIniMovimientoHumano = clock_get_us(); 
					} else if (estadoActual == VICTORIA) {
						if (ID_Evento == ev_TX_SERIE) {// si me llega el evento de qye se ha acabado de printear el tablero  y estoy en victoria se imprime toda la informacion.
								finalizarPartida(1,jugadorAnterior, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
								estadoActual=ESTADISTICAS;
						}
					}else if (estadoActual == RENDICION) {
						if (ID_Evento == ev_TX_SERIE) {// si me llega el evento de qye se ha acabado de printear el tablero  y estoy en victoria se imprime toda la informacion.
								finalizarPartida(1,jugadorAnterior, nuevoVector, tiempoTotalPartidaIni, tiempoTotalHayLinea, tiempoTotalHumano, numCallsHayLinea, numJugadasHumano);
								estadoActual=ESTADISTICAS;
						}
					}
					else if (estadoActual == EMPATE) {
						if (ID_Evento == ev_TX_SERIE) {// si me llega el evento de qye se ha acabado de printear el tablero  y estoy en victoria se imprime toda la informacion.
								partidaEmpatada(nuevoVector);
								estadoActual=ESTADISTICAS;
						}
					}
					else if (estadoActual == PREVIEW_MOVE || estadoActual == ESPERAR_JUGADA || estadoActual == EXEC_MOVE || estadoActual==ERROR_CMD){
						// si me llega eint2 y estoy en alguno de esos estados se rinde
						if(auxData == EINT2){
							alarma_activar(ev_confirmarJugada, 0, 0);
							estadoActual = RENDICION;
							mensajeGanador(0, jugadorAnterior, nuevoVector);
						}
				}
					else if(estadoActual==ESTADISTICAS){
						if(ID_Evento==ev_TX_SERIE){
							estadoActual=IDLE;						}
					}
}
