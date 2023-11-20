#include "conecta_K_2023.h"
#include "entrada.h"
// carga el estado a mitad de partida de las primeras 7 filas y columnas 
// a la estructura de datos tablero para facilitar el test de posibles jugadas
//
// 0: casilla vacia, 1:ficha jugador uno, 2: ficha jugador dos
void conecta_K_test_cargar_tablero(TABLERO *t)
{
#include "tablero_test.h"

	for (size_t i = 0; i < NUM_FILAS; i++)
	{
		for (size_t j =0; j <NUM_COLUMNAS; j++)
		{
			if (tablero_test[i][j] != 0){
			//CELDA nueva = tablero_test[i][j];
			tablero_insertar_color(t,i,j,tablero_test[i][j]);
				}
		}
	}
}

// funcion que visualiza en "pantalla" el contenido de las 7 primeras filas y columnas 
// de las m*n del tablero en juego (suponemos que m y n son >= 7 siempre)
// en memoria se deberia ver algo tal que asi:
// 00 C1 C2 C3 C4 C5 C6 C7
// F1 00 00 00 00 00 00 00
// F2 00 00 11 00 00 00 00
// F3 00 11 22 22 00 00 00
// F4 00 00 11 00 00 00 00
// F5 00 00 00 00 00 00 00
// F6 00 00 00 00 00 00 00
// F7 00 00 00 00 00 00 00 
void conecta_K_visualizar_tablero(TABLERO *t, uint8_t pantalla[8][8])
{
	for (size_t i =0; i < 8 ; i++){
		for (size_t j = 0; j < 8; j++) {
			//casos a cubrir: tengo i y j al entrar aqui
			//esquina superior izda caso especial
			if (i==0 && j==0) {
				pantalla[i][j] = 0xFC;
			}
			//columna 0: F + numI
			else if (i!=0 && j==0){
				pantalla[i][j] = 0xF0 + i;
			}
			//fila 0: C + numJ
			else if (i==0 && j!=0) {
				pantalla[i][j] = 0xC0 + j;
			}
			//resto de casos: leer tablero y poner info
			else {
				CELDA escribir = tablero_leer_celda(t,i-1,j-1);
				uint8_t c = celda_color(escribir);
				if (c == 0){
					pantalla[i][j] = 0x00;
				} else if (c==1){
					pantalla[i][j] = 0x11;
				} else {
					pantalla[i][j] = 0x22;
				}
			}
		}
	} 
} 

/*
int conecta_K_verificar_K_en_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color){

	uint8_t resultado_arm_arm = conecta_K_hay_linea_arm_armv2(t, fila, columna, color);
	return resultado_arm_arm;
}
*/
void inicializarDiagonal(uint8_t autojugada[][3]) {
			autojugada[0][0]=3;
			autojugada[0][1]=5;
			autojugada[0][2]=2;
			autojugada[1][0]=7;
			autojugada[1][1]=7;
			autojugada[1][2]=1;
			autojugada[2][0]=4;
			autojugada[2][1]=4;
			autojugada[2][2]=2;
}	
void inicializarVertical(uint8_t autojugada[][3]) {
			autojugada[0][0]=4;
			autojugada[0][1]=5;
			autojugada[0][2]=2;
			autojugada[1][0]=2;
			autojugada[1][1]=3;
			autojugada[1][2]=1;
			autojugada[2][0]=1;
			autojugada[2][1]=1;
			autojugada[2][2]=2;
			autojugada[3][0]=1;
			autojugada[3][1]=3;
			autojugada[3][2]=1;
			autojugada[4][0]=7;
			autojugada[4][1]=4;
			autojugada[4][2]=2;
			autojugada[5][0]=4;
			autojugada[5][1]=3;
			autojugada[5][2]=1;
}	
void inicializarHorizontal(uint8_t autojugada[][3]) {
			autojugada[0][0]=5;
			autojugada[0][1]=4;
			autojugada[0][2]=2;
			autojugada[1][0]=7;
			autojugada[1][1]=3;
			autojugada[1][2]=1;
			autojugada[2][0]=6;
			autojugada[2][1]=5;
			autojugada[2][2]=1;
}	
void conecta_K_jugar(void){
	/*// new, row, column, colour, padding to prevent desalinating to 8 bytes
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...
	// 8x8 intentando que este alineada para que se vea bien en memoria
	static uint8_t salida[8][8];
		static uint8_t autoJugada[3][3];
	int JugarAutomatico=0;
	if(JugarAutomatico == 1){
		//inicializarDiagonal(autoJugada);
		//inicializarHorizontal(autoJugada);
		inicializarVertical(autoJugada);
	}
	TABLERO cuadricula;

	//uint8_t row, column, colour;

	tablero_inicializar(&cuadricula);

	conecta_K_test_cargar_tablero(&cuadricula);
	//conecta_K_visualizar_tablero(&cuadricula, salida);

	entrada_inicializar(entrada);
	//int i=1;
	*/
	/*
	while (1){
		if(JugarAutomatico == 1){
			entrada[0]=1;
			entrada[1]=autoJugada[i][0];
			entrada[2]=autoJugada[i][1];
			entrada[3]=autoJugada[i][2];
		}
		i++;
		while (entrada_nueva(entrada) == 0){};
		entrada_leer(entrada, &row, &column, &colour);
		//validada la entrada en rango, mirar color valido?
		if(tablero_fila_valida(row) && tablero_columna_valida(column) && tablero_color_valido(colour)){	
			//podriamos no validarla ya que tablero_insertar_valor vuelve a validar
			if (celda_vacia(tablero_leer_celda(&cuadricula, row, column))){
				//tablero_insertar tambien chequea si esta libre esa celda o no...
				if(tablero_insertar_color(&cuadricula, row, column, colour) == EXITO) {
					conecta_K_visualizar_tablero(&cuadricula, salida);
					if(conecta_K_verificar_K_en_linea(&cuadricula, row, column, colour)) {
						while(1); // equivaldria a K_linea encontrada, fin de partida... 
					}
				}
				else {
					while(1); //no cabe en la matriz dispersa, hemos dimensionado mal, error de diseño
				}
			}
			//else: celda no vacia
		}
		//else: fuera de rango fila, columna o color
		entrada_inicializar (entrada);
	}*/
}
