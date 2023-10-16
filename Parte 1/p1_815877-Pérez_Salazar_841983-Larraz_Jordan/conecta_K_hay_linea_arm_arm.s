K_SIZE EQU 4		;.include "config_conecta_k.h" MIRAR COMO INCLUIR GLOBALES DE C EN ENSAMBLADOR
N_DELTAS EQU 4					; para recorrer en bucle
NUM_FILAS EQU 7
NUM_COLUMNAS EQU 7
MAX_NO_CERO EQU 6
EXITO  EQU 0x0	
	AREA datos,DATA,READWRITE
deltas_fila DCB 0x00,0xff,0xff,0x01 ; declaracion del vector de filas
deltas_columna DCB 0xff,0x00,0xff,0xff  ; declaracion del vector de columnas

	AREA prog,CODE,READONLY
	ENTRY

	EXPORT conecta_K_hay_linea_arm_arm
	IMPORT conecta_K_buscar_alineamiento_arm
	PRESERVE8 {TRUE}

conecta_K_hay_linea_arm_arm

	stmdb r13!, {r4-r10, fp, lr}	; apilo los registros que podria machacar
	mov r4, r0	; r4=t*
	mov r10, r3
	mov r5, #0 	; r5= i
	;comienza el bucle de la funcion hay_linea
forarmarm cmp r5, #N_DELTAS 
	bge finalFalse
	mov r6,#0	;r6=longlinea
	ldr r9, =deltas_fila; cargo la direccion de deltas_fila
	ldrsb r7, [r9,r5]; r7 = deltas_fila i
	ldr r9, =deltas_columna; cargo la direccion de deltas_columna
	ldrsb r8, [r9, r5]; r8 = deltas_columna i
;comienza buscar_alineamiento_arm
	stmdb sp!,{r1-r2}
	stmdb sp!,{r7-r8}		;metemos params en pila (REVISAR ORDEN)
	mov r0, r4
	mov r3, r10
	bl conecta_K_buscar_alineamiento_arm
	mov r6, r0;mover res a reg libre
	add sp,sp,#8
	ldmia sp!,{r1-r2}
	cmp r6, #K_SIZE ; comparamos long linea con ksize
	bge finalTrue	; continue (lo mandamos al final para evitar comprobaciones innecesarias que se hacen en c antes de entrar al bucle)
;termina buscar_alineamiento_arm
	stmdb sp!, {r1-r2}	; guardo valores de fila y columna en pila
	rsb r7, r7, #0		;-delta_fila
	rsb r8, r8, #0		;-delta_columna
	add r1, r1, r7		;updatefila
	add r2, r2, r8		;updatecolumna
;comienza buscar_alineamiento_arm2
	mov r0, r4
	mov r3, r10
	stmdb sp!,{r7-r8}		;metemos params en pila (REVISAR ORDEN)
	bl conecta_K_buscar_alineamiento_arm
	add r6, r6, r0			;sumar res a longlinea
	add sp,sp,#8
;termina buscar_alineamiento_arm
	ldmia sp!, {r1-r2}	; restauro valores de fila y columna para siguiente iteracion
	cmp r6, #K_SIZE
	addlt r5,r5,#1 ; dejo i actualizado con i++ para la siguiente iteracion
	blt forarmarm; vuelve al for de hay linea forarmarm
finalTrue mov r0, #1 ;; HA FINALIZADO POR LINEA == true
	ldmia sp!, {r4-r10,fp, lr}
	bx lr
finalFalse mov r0, #0
	ldmia sp!, {r4-r10, fp, lr} ;;HA FINALIZADO POR I >= n_deltas
	bx lr
	END