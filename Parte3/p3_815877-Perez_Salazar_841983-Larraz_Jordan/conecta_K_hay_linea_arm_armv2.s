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

	EXPORT conecta_K_hay_linea_arm_armv2
	PRESERVE8 {TRUE}

conecta_K_hay_linea_arm_armv2

	stmdb r13!, {r4-r10,ip, fp, lr}	; apilo los registros que podria machacar
	sub fp, sp, #4		; muevo el fp para que apunte a la dirección libre
	stmdb sp!, {r1-r3}	; apilo los parametros fila, columna y color 
	
	mov r4, r0	; r4=t*
	mov r1, #0 	; r1= i
	;comienza el bucle de la funcion hay_linea
forarmarm cmp r1, #N_DELTAS 
	bge finalFalse
	mov r2,#0	;r2=longlinea
;comienza buscar_alineamiento_arm
; esto es buscar_color
	ldmia sp, {r5,r6}	; cargo en r5 y r6 fila y columna
	ldr r7, =deltas_fila; cargo la direccion de deltas_fila
	ldrsb r10, [r7,r1]; r2 = deltas_fila i
	ldr r7, =deltas_columna; cargo la direccion de deltas_columna
	ldrsb r12, [r7, r1]; r3 = deltas_columna i
whileColor	cmp r5, #NUM_FILAS	;si me paso de fila o col, es invalido
	bge no_color1;

	cmp r6, #NUM_COLUMNAS
	bge no_color1;	columnas > NUM_COLUMNAS?

	mov r3, #0				; r3(col) = 0
for1 cmp r3, #MAX_NO_CERO
	bge fuera1
	add r7, r5, r5, LSL #1	; r7 = fila + fila*2
	add r7, r4, r7, LSL #1	; r7 = t +r7*2
	ldrb r8, [r7, r3]	; r8 = t->columnas[fila][col]
	cmp r8, r6	; comparo con columna
	addne r3, r3, #1 ; si son diferentes sumo col++ para la siguiente iteracion
	bne for1

fuera1 cmp  r3, #MAX_NO_CERO
	beq no_color1

	add r7,r5, r5, LSL #1  ; r7 = fila + fila*2
	add r8, r4, #0x0000002a	; r8 = t + 2a (para acceder a no_ceros)
	add r7, r8, r7, LSL #1	;r7 = r8 + r7*2
	ldrb r9, [r7, r3] ; r9 = t->no_ceros[fila][col]
	and r9, r9, #0x03	; operacion and para ver si es vacia o no
	ldmia fp, {r7}			; cargo color en r7
	cmp r9, r7				; comparo con el color q he sacado
	bne no_color1	
	;fin tablero_buscar_color (si llegas a esta linea es exito)
	add r5, r5, r10	; hago la suma con nueva fila 
	add r6, r6, r12	 ; hago la suma con nueva col 
	add r2, r2, #1	; sumo un uno a long_linea que sera lo q devolvamos
	b whileColor		;bucle fin de buscar_alineamiento
;termina buscar_alineamiento_arm

no_color1	cmp r2, #K_SIZE ; comparamos long linea con ksize
	bge finalTrue
	
	ldmia sp, {r5,r6}	; cargo fila y columna
	rsb r10, r10, #0		;-delta_fila
	rsb r12, r12, #0		;-delta_columna
	add r5, r5, r10			;updatefila
	add r6, r6, r12			;updatecolumna

;comienza buscar_alineamiento_arm2
; esto es buscar_color
whileColor2	cmp r5, #NUM_FILAS	;si me paso de fila o col, es invalido
	bge no_color2;

	cmp r6, #NUM_COLUMNAS
	bge no_color2;	columnas > NUM_COLUMNAS?

	mov r3, #0				; r3(col) = 0
for2 cmp r3, #MAX_NO_CERO
	bge fuera2
	add r7, r5, r5, LSL #1	; r7 = fila + fila*2
	add r7, r4, r7, LSL #1	; r7 = t +r7*2
	ldrb r8, [r7, r3]	; r8 = t->columnas[fila][col]
	cmp r8, r6	; comparo con columna
	addne r3, r3, #1 ; si son diferentes sumo col++ para la siguiente iteracion
	bne for2

fuera2 cmp  r3, #MAX_NO_CERO
	beq no_color2

	add r7,r5, r5, LSL #1  ; r7 = fila + fila*2
	add r8, r4, #0x0000002a	; r8 = t + 2a (para acceder a no_ceros)
	add r7, r8, r7, LSL #1	;r7 = r8 + r7*2
	ldrb r9, [r7, r3] ; r9 = t->no_ceros[fila][col]
	and r9, r9, #0x03	; operacion and para ver si es vacia o no
	ldmia fp, {r7}			; cargo color en r7
	cmp r9, r7				; comparo con el color q he sacado
	
	bne no_color2	
	;fin tablero_buscar_color (si llegas a esta linea es exito)
	add r5, r5, r10	; hago la suma con nueva fila 
	add r6, r6, r12	 ; hago la suma con nueva col 
	add r2, r2, #1	; sumo un uno a long_linea que sera lo q devolvamos
	b whileColor2		;bucle fin de buscar_alineamiento
	;termina buscar_alineamiento_arm

no_color2	cmp  r2, #K_SIZE
	addlt r1,r1,#1 ; dejo i actualizado con i++ para la siguiente iteracion
	blt forarmarm; vuelve al for de hay linea forarmarm
	
finalTrue mov r0, #1 ;; HA FINALIZADO POR LINEA == true
	add sp,sp,#12 ;deshacer stmdb sp!, {r1-r3}
	ldmia sp!, {r4-r10,ip, fp, lr}
	bx lr
finalFalse mov r0, #0
	add sp,sp,#12
	ldmia sp!, {r4-r10,ip, fp, lr} ;;HA FINALIZADO POR I >= n_deltas
	bx lr
	END