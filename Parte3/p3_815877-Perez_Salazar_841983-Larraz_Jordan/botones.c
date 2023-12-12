#include "botones.h"
static void (*encolar)()=NULL;
EVENTO_T miEventoBoton, boton1, boton2, botonTemporizador; 
void botones_ini(void (*funcion_encolar_evento)(), EVENTO_T _miEventoBoton, EVENTO_T _boton1, EVENTO_T _boton2, EVENTO_T _botonTemporizador){
	miEventoBoton=_miEventoBoton;
	boton1=_boton1;
	boton2=_boton2;
	botonTemporizador=_botonTemporizador;
	void (*funcion_callback)(uint32_t) = esPulsado;
	encolar = funcion_encolar_evento;
	eint_init(funcion_callback);
}

void esPulsado(uint32_t id_boton){
	if(id_boton == 1){
		if(estaPulsadoEint1()==1){
			encolar(miEventoBoton, boton1);
			alarma_activar(botonTemporizador, 0x80000064,boton1);
			estado1 = 1;
		}
	}
	else if(id_boton == 2){
		if(estaPulsadoEint2()==1){
			encolar(miEventoBoton, boton2);
			alarma_activar(botonTemporizador, 0x80000064,boton2);
			estado2 = 1;
		}
	}
}

void comprobarEstado(void){
		if(estado1==1){
			if(estaPulsadoEint1()==0){
				if(estado2==0){
					alarma_activar(botonTemporizador, 0x0,boton1);
				}
				habilitar_irq_eint1();
				estado1=0;
			}	
		}
		if(estado2==1){
			if(estaPulsadoEint2()==0){
				if(estado1==0){
					alarma_activar(botonTemporizador, 0x0,boton2);
				}
				habilitar_irq_eint2();
				estado2=0;
			}
	}
}

