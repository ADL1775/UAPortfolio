; Lab14.asm
;   1- Define and declare a structure Point that has x and y coordinates
;   2- Define an array of 5 structures of Point type
;   3- Use loop 1 to ask a user to input x and y values for the 5 structures using system calls
;   4- Use a loop 2 to print out the x and y values for the array of structure using the system calls             

STRUC Point
    .x:     resd 1
    .y:     resd 1
    .size:
ENDSTRUC

section .data
	; Define variables here
    msg1:       db "You will set the x and y values for the three points",10,0
    msgL1:      equ $-msg1

    msg2:       db "Printing the point coordinates... ",10,0
    msgL2:      equ $-msg2

    msg3:       db "Enter a value for coodinate x",0
    msgL3:      equ $-msg3

    msg4:       db  "Enter a value for coodrinate y",0
    msgL4:      equ $-msg4
    
    msgTest:    db  "You entered: ",10,0
    msgTestL:   equ $-msgTest

    msgCoord:   db  "Printing array of structures' coordinates:",10,0
    msgCoordL:  equ $-msgCoord

    msgCNum:    db  "X and Y coordinates of array",0
    msgCNumL:   equ $-msgCNum


    ; Initialize the structure
    P1: ISTRUC Point
        AT Point.x, dd 0
        AT Point.y, dd 0
    IEND
    
    coordNum: dd 1

    userX:      dd 0
    userY:      dd 0
    loopCount:  dd 1

section .bss
	; Reserve variable data here.
    PtArr:  resb Point.size*5   ; Reserve place for 5 structures
                                ; PtArr is a pointer to the start of all arrays

    ArrCount: EQU ($-PtArr)/Point.size 


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

    call    CoordInput
    call    Println

    mov     ecx, msgCoord
    mov     edx, msgCoordL
    call    PrintString

    mov     dword[coordNum], 1

    mov     esi, PtArr          ; ESI has the address of the first structure in the array

; Print the values
L2:
    mov     ecx, msgCNum
    mov     edx, msgCNumL
    call    PrintString
    mov     eax, dword[coordNum]
    call    PrintDec
    call    Println
    
    mov     eax, [esi + Point.x]        ; Indirect access to X value
    call    PrintDec
    call    Println

    mov     eax, [esi + Point.y]        ; Indirect access to Y value
    call    PrintDec
    call    Println

    add     esi, Point.size             ; Move to the next structure in the array 

    inc     dword[coordNum]
    cmp     dword[coordNum], 5
    jle     L2

    ; Destroy the stack and exit the program
    mov     esp, ebp
    pop     ebp
    ret
    


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CoordInput:
    pusha
    mov     esi, PtArr
Top1:
    ; Print X coord message
    mov     ecx, msg3
    mov     edx, msgL3
    call    PrintString
    mov     eax, dword[coordNum]
    call    PrintDec
    call    Println

    ; Input X coord
    mov     eax, 3
    mov     ebx, 0
    mov     ecx, userX
    int     80h


    mov     eax, userX
    call    TrimInput

    lea     edi, [userX]
    call    StringToInt
    mov     [esi + Point.x], eax

    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; TESTING
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov     ecx, msgTest
    mov     edx, msgTestL
    call    PrintString

    mov     dword[userX], eax
    call    PrintDec
    call    Println
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; Print Y coord message
    mov     ecx, msg4
    mov     edx, msgL4
    call    PrintString
    mov     eax, dword[coordNum]
    call    PrintDec
    call    Println

    ; Input Y coord
    mov     eax, 3
    mov     ebx, 0
    mov     ecx, userY
    int     80h

    mov     eax, userY
    call    TrimInput

    lea     edi, [userY]
    call    StringToInt
    mov     [esi + Point.y], eax

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; TESTING
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov     ecx, msgTest
    mov     edx, msgTestL
    call    PrintString

    mov     dword[userY], eax
    call    PrintDec
    call    Println
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    ;mov     esi + Point.y, [userY]
    inc     dword[coordNum]
    add     esi, Point.size
    cmp     dword[coordNum], 5
    jle     Top1

    popa
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;https://stackoverflow.com/questions/19309749/nasm-assembly-convert-input-to-integer
StringToInt:

    push    ebp
    mov     ebp, esp
    ;mov     ecx, 1
    xor     ebx, ebx

NextDigit:
    
    movzx   eax, byte[edi]
    
    inc     edi
    cmp     eax, 0
    je      EndConv

    sub     al, '0'
    imul    ebx, 10
    add     ebx, eax
    jmp     NextDigit

EndConv:
    mov     eax, ebx

    mov     esp, ebp
    pop     ebp
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TrimInput:
section .text
    pusha                       ; Save all registers
    mov     edi, eax            ; EDI points to the first byte of the user-input
    inc     edi                 ; Moved to the next byte of the address

TrimTop:
    cmp     byte[edi], 0ah
    je      TrimNewline

TrimMid:
    inc     edi                 ; Move to next character in string
    inc     dword[loopCount]

    cmp     dword[loopCount], 4 ; Is byte that EDI is pointing to = 0? (The end of the integer)
    jne     TrimTop             ; No, keep on looping

TrimEnd:
    mov     dword[loopCount], 1
    ; Restore all registers
    popa
    ret

TrimNewline:
    mov     byte[edi], 0
    jmp     TrimMid
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;






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
