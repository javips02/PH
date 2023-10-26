K_SIZE EQU 4		;.include "config_conecta_k.h" MIRAR COMO INCLUIR GLOBALES DE C EN ENSAMBLADOR
N_DELTAS EQU 4		
	AREA datos,DATA,READWRITE

deltas_fila DCB 0x00,0xff,0xff,0x01
deltas_columnas DCB 0xff,0x00,0xff,0xff

	AREA prog,CODE,READONLY
	ENTRY
	IMPORT conecta_K_buscar_alineamiento_c
	EXPORT conecta_K_hay_linea_arm_c
	PRESERVE8 {TRUE}

conecta_K_hay_linea_arm_c
	stmdb r13!, {r4-r10, fp, lr} ; apilamos registros que podriamos machacar
	mov r4, r0; r4=t
	mov r5, r1; r5 = fila
	mov r6, r2; r6 = columna
	mov r7, r3; r7 = color

	mov r8, #0 ; r8 = i =0
	mov r9 , #0 ; r9 = long_linea
for cmp r8, #N_DELTAS ; r8 < N_DELTAS?
	bge finFalse
	ldr r0, =deltas_fila; r0 = direccion de deltas_fila
	ldr r1, =deltas_columnas ; r1 = direccion de deltas_columna
	ldrsb r2, [r0,r8]; r2 = deltas_fila i
	ldrsb r3, [r1, r8]; r3 = deltas_columna i
	stmdb sp!, {r2, r3} ; apilo como parametros
	mov r0, r4 ; copiade parametros para pasarlos
	mov r1, r5
	mov r2, r6
	mov r3, r7;
	bl conecta_K_buscar_alineamiento_c
	ldmia sp!, {r2,r3} ; desapilo deltas que usare
	cmp r0, #K_SIZE	; comparo para saber si linea es true
	bge finTrue
	mov r9, r0	; lo muevo para almacenar el resultado
	rsb r2, r2, #0	; -delatFIlai
	rsb r3, r3, #0; -delatColumna i
	stmdb sp!, {r2,r3}; apilo deltas como parametro
	mov r0, r4
	add r1, r5, r2	; updatefila
	add r2, r6, r3; updatecolumna
	mov r3, r7;
	bl conecta_K_buscar_alineamiento_c
	add sp, sp, #8; libero pila
	add r9, r9, r0;	incremento long linea con lo que haya salido
	cmp  r9, #4
	bge finTrue
	add r8, r8, #1 ;i++
	b for
	
finTrue mov r0, #1	; final que devolvera un uno porque linea es true
	ldmia sp!, {r4-r10, fp, lr}
	bx lr
finFalse mov r0, #0 ; final que devuelve false porque i > N_DELTAS
	ldmia sp!, {r4-r10, fp, lr}
	bx lr
	END