q;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/
I_Bit           EQU     0x80   
T_Bit           EQU     0x20

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number

; add code to enable/disable the global IRQ flag
                //CMP     R12,#0xFF              
                //BEQ     __decrease_var
				
				CMP     R12,#0xFF              
                BEQ     __enable_irq
				
				CMP     R12,#0xFE              
                BEQ     __disable_irq
				
				CMP     R12,#0xFD             
                BEQ     __disable_fiq
				
				CMP     R12,#0xFC             
                BEQ     __read_IRQ_bit

                LDR     R8, SWI_Count
                CMP     R12, R8
                BHS     SWI_Dead               ; Overflow
                ADR     R8, SWI_Table
                LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address
                MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 

                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

SWI_Dead        B       SWI_Dead               ; None Existing SWI

SWI_Cnt         EQU    (SWI_End-SWI_Table)/4
SWI_Count       DCD     SWI_Cnt

                IMPORT  __SWI_0					; tick_getus decalrada en el módulo temporizador_drv.h
;               IMPORT  __SWI_1
;               IMPORT  __SWI_2
;               IMPORT  __SWI_3
SWI_Table
                DCD     __SWI_0                ; SWI 0 Function Entry
;               DCD     __SWI_1                ; SWI 1 Function Entry
;               DCD     __SWI_2                ; SWI 2 Function Entry
;               DCD     __SWI_3                ; SWI 3 Function Entry

;               ...
SWI_End

__decrease_var
                LDR R8, =shared_var
				LDR R12, [r8]
                SUB R12, R12, #1
                STR R12, [R8]
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

__read_IRQ_bit
				MRS R0, SPSR        ; Mueve el contenido del registro SPSR a R0 
				AND R0, R0, #I_Bit  ; Realiza una operación AND para aislar el bit IRQ
				CMP R0, #0          ; Compara el resultado con 0 (IRQ's activadas porque flag a 0)
				MOVEQ R0, #0		; return 0 si estan activadas
				MOVNE r0, #1		; return 1 si estan desactivadas
				LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				; En el caso de esta funcion, devolvemos el resultado en r0 (un 0 si estan activadas, un 1 si estan desactivadas, copiando el signiicado del flag)

__enable_irq
				MRS R8, SPSR        ; Mueve el contenido del registro SPSR a R8 
				BIC R8, R8, #I_Bit  ; Realiza una operación BIC para borrar el bit IRQ en R8
				MSR SPSR_cxsf, R8       ; Establece el nuevo valor de CPSR con el bit IRQ borrado
				LDMFD SP!, {R8, R12}    ; Load R8, SPSR (restaurar pila antes de volver) (en este caso, el spsr lo hemos modificado ya con lo que nos interesaba, por lo que no lo restauramos)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				
__disable_irq
				MRS R8, SPSR        ; Mueve el contenido del registro SPSR a R8 
				ORR R8, R8, #I_Bit  ; Realiza una operación BIC para levantar el bit IRQ en R8
				MSR SPSR_cxsf, R8       ; Establece el nuevo valor de CPSR con el bit IRQ levantado
				LDMFD SP!, {R8, R12}    ; Load R8, SPSR (restaurar pila antes de volver) (en este caso, el spsr lo hemos modificado ya con lo que nos interesaba, por lo que no lo restauramos)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				
__disable_fiq
				MRS R8, SPSR        ; Mueve el contenido del registro SPSR a R8 
				ORR R8, R8, #F_Bit  ; Realiza una operación BIC para levantar el bit IRQ en R8
				MSR SPSR_cxsf, R8       ; Establece el nuevo valor de CPSR con el bit FIQ levantado
				LDMFD SP!, {R8, R12}    ; Load R8, SPSR (restaurar pila antes de volver) (en este caso, el spsr lo hemos modificado ya con lo que nos interesaba, por lo que no lo restauramos)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

                END

