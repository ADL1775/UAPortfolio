section .bss

section .data
	; Define variables here.
    z       dd  123456789
    z1      dd  232
    z2      dd  434
    z3      dd  0
    msgZ    db  "Z = ",0


; Code goes in the text section
section .text
	GLOBAL main

main:
	push    ebp
    mov     ebp, esp
    call    Println


    ; Before calling Pstring Function...
    ; Assign the required registers with the proper values...
    ;   ...to display the message

    ; Display int msg and value
    mov     ecx, msgZ
    mov     edx, 4
    call    PrintString;
    mov     eax, [z]

    call    PrintDec
    call    Println

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; NEW CODE
    mov     eax, z1
    mov     eax, [z1]
    call    PrintDec
    call    Println

    mov     eax, [z2]
    call    PrintDec
    call    Println

    mov     eax, [z1]
    add     eax, [z2]
    mov     DWORD [z3], eax

    call    PrintDec
    call    Println

    ; END NEW CODE
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Terminate program
    mov     esp, ebp
    pop     ebp
    ret

PrintString:
    ; Save the register values of the called function
    pusha

    ; String is pointed by ECX, EDX has its length.
    mov     eax, 4
    mov     ebx, 1
    int     80h

    ; Restore the old register values of the called function
    popa
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Println:
    ; We will call PrintString function
    ; That will change the content of ecx and edx
    ; We need to save registers used by the main program
    section .data
        n1      db      10

    section .text
        pusha

        mov     ecx, n1
        mov     edx, 1
        call    PrintString

        ; Return original register values.
        popa
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PrintDec:
    ; Saves all the registers so that they are not changed by the function
    ; We build the function to handle the dword size (4 bytes)
    section .bss
        decstr      resb    10      ; 10 digit number for 32 bits
        ct1         resd    1       ; To keep track of the size of the dec-string

    section .text
        pusha       ; Save all registers

        mov     edx, ct1
        mov     dword[ct1], 0       ; Initialize to zero
        mov     edi, decstr         ; EDI points to the dec-string in memory
        add     edi, 9              ; Moved to the last element of the string
        xor     edx, edx            ; Clear EDX for 64-bit division


WhileNotZero:
    mov     ebx, 10             ; Get ready to divide by 10
    div     ebx                 ; Divide by 10

    add     edx, '0'            ; Converts to ASCII character

    mov     byte[edi], dl       ; Put it in string

    dec     edi                 ; Move to next character in string

    inc     dword[ct1]          ; Increment character counter

    xor     edx, edx            ; Clear out EDX

    cmp     eax, 0              ; Is the remainder of the division 0?
    jne     WhileNotZero        ; No, keep on looping

    inc     edi                 ; Conversion, finish, bring EDI
    mov     ecx, edi            ; Back to beginning of string. Make ECX

    mov     edx, [ct1]          ; Point to it, and EDX gets # of chars

    mov     eax, 4
    mov     ebx, 1

    int     80h

    ; Restore all registers
    popa
    ret




