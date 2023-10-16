NUM_FILAS EQU 7		
NUM_COLUMNAS EQU 7
MAX_NO_CERO EQU 6
EXITO  EQU 0x0
	AREA datos,DATA,READWRITE

	AREA prog,CODE,READONLY
	ENTRY
	IMPORT tablero_buscar_color
	EXPORT conecta_K_buscar_alineamiento_arm
	PRESERVE8 {TRUE}				; solución temas de alineamiento y datos que vienen de C

conecta_K_buscar_alineamiento_arm
	stmdb sp!, {r4-r10, fp, ip,lr}
	mov r4, r0; r4=t
	mov r5, r1; r5 = fila
	mov r6, r2; r6 = columna
	mov r7, r3; r7 = color
; empieza Tablero_buscar_color
	cmp r5, #NUM_FILAS	; comparo r5 con munfilas para saber si es incorrecto
	movge r0, #0x00
	bge final;

	cmp r6, #NUM_COLUMNAS;comparo r5 con numColumnas para saber si es incorrecto
	movge r0, #0x00
	bge final;

	mov r10, #0; col = 0

for1 cmp r10, #MAX_NO_CERO ; bucle que encuentra la columna de la ficha a analizar el color
	bge fuera

	add r0, r5, r5, LSL #1
	add r0, r4, r0, LSL #1
	ldrb r1, [r0, r10]
	cmp r1, r6
	addne r10, r10, #1
	bne for1
fuera
	cmp  r10, #MAX_NO_CERO
	moveq r0, #0x00
	beq final;

	add r1,r5, r5, LSL #1 
	add r2, r4, #0x0000002a
	add r1, r2, r1, LSL #1
	LDRB r0, [r1, r10]
	AND r0, r0, #0x03 ; se compara el color sacado  
	cmp r0, r7
	movne r0, #0x00
	bne final;
	;termina tablero_buscar_color

	ldmia ip, {r8,r9} ; cargamos los deltas
	add r1, r5, r8	; nueva fila
	add r2, r6, r9 ; nueva columna
	stmdb sp!, {r8, r9}	; apilo deltas
	mov r0, r4
	mov r3, r7
	bl conecta_K_buscar_alineamiento_arm
	add sp, sp, #8; libero pila
	add r0,r0, #0x01 ;  1 + la llamada recursiva
final ldmia sp!, {r4-r10, fp, ip,lr}
	mov sp, ip
	bx r14

	END