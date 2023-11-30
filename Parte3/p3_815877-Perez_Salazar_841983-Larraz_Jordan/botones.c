
#include "botones.h"
static void (*encolar)()=NULL;
void botones_ini(void (*funcion_encolar_evento)()){
	
	void (*funcion_callback)(uint32_t) = esPulsado;
	encolar = funcion_encolar_evento;
	eint_init(funcion_callback);
}

void esPulsado(uint32_t id_boton){
	if(id_boton == EINT1){
		if(estaPulsadoEint1()==1){
			encolar(BOTON, id_boton);
			alarma_activar(BOTON_TEMPORIZADOR, 0x80000064,EINT1);
			estado1 = 1;
		}
	}
	else if(id_boton == EINT2){
		if(estaPulsadoEint2()==1){
			encolar(BOTON, id_boton);
			alarma_activar(BOTON_TEMPORIZADOR, 0x80000064,EINT2);
			estado2 = 1;
		}
	}
}

void comprobarEstado(void){
		if(estado1==1){
			if(estaPulsadoEint1()==0){
				if(estado2==0){
					alarma_activar(BOTON_TEMPORIZADOR, 0x0,EINT1);
				}
				habilitar_irq_eint1();
				estado1=0;
			}	
		}
		if(estado2==1){
			if(estaPulsadoEint2()==0){
				if(estado1==0){
					alarma_activar(BOTON_TEMPORIZADOR, 0x0,EINT2);
				}
				habilitar_irq_eint2();
				estado2=0;
			}
	}
}

