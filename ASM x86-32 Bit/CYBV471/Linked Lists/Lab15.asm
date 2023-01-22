; Lab14.asm
;   1- Define and declare a structure Point that has x and y coordinates
;   2- Define an array of 5 structures of Point type
;   3- Use loop 1 to ask a user to input x and y values for the 5 structures using system calls
;   4- Use a loop 2 to print out the x and y values for the array of structure using the system calls             

STRUC Node
    .Value:     resd 1
    .NextPtr:   resd 1
    .PrevPtr:   resd 1
ENDSTRUC

section .data
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Initialize the structure
    Head: ISTRUC Node
        AT Node.Value, dd 1
        AT Node.NextPtr, dd Second
        AT Node.PrevPtr, dd 0
    IEND


    Second: ISTRUC Node
        AT Node.Value, dd 2
        AT Node.NextPtr, dd Third
        AT Node.PrevPtr, dd Head
    IEND

    Third: ISTRUC Node
        AT Node.Value, dd 3
        AT Node.NextPtr, dd Tail
        AT Node.PrevPtr, dd Second
    IEND


    Tail: ISTRUC Node
        AT Node.Value, dd 4
        AT Node.NextPtr, dd 0
        AT Node.PrevPtr, dd Third
    IEND
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; Define variables here
    msg1:       db "Printing the linked list information",10,0
    msgL1:      equ $-msg2

    msg2:       db "Node num and value: ",0
    msgL2:      equ $-msg2

    msg3:       db "Node mem location: ",0
    msgL3:      equ $-msg3

    msg4:       db "Next node mem location: ",0
    msgL4:      equ $-msg4

    msg5:       db "Previous node mem location: ",0
    msgL5:      equ $-msg5

section .bss

; Code goes in the text section
section .text
	GLOBAL main

main:   
    ; Build the stack.
	push	ebp
	mov		ebp, esp    

    ; Print the message informing of program purpose
    mov     ecx, msg1
    mov     edx, msgL1
    call    PrintString

    mov     edi, Head
    call    PrintNode

    mov     edi, Second
    call    PrintNode

    mov     edi, Third
    call    PrintNode

    mov     edi, Tail
    call    PrintNode

    ;Exit
    mov     esp, ebp
    pop     ebp
    ret

PrintNode:
    pusha

    ; Print node value contents
    mov     ecx, msg2       
    mov     edx, msgL2
    call    PrintString
    mov     eax, [edi]     
    call    PrintDec
    call    Println
    
    ; Print node memory location
    mov     ecx, msg3       
    mov     edx, msgL3
    call    PrintString
    mov     eax, edi       
    call    PrintDec
    call    Println


    ; Print next node memory location
    mov     ecx, msg4       
    mov     edx, msgL4
    call    PrintString
    mov     eax, [edi + Node.NextPtr]    ; Pointer value
    call    PrintDec
    call    Println

    ; Print previous node memory location
    mov     ecx, msg5       
    mov     edx, msgL5
    call    PrintString
    mov     eax, [edi + Node.PrevPtr]
    call    PrintDec
    call    Println
    call    Println

    popa
    ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
