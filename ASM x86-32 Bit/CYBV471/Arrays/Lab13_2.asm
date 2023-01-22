; Lab13_2.asm
;   1- Create a file “file1.txt” that has your name
;   2- Using system calls, write assembly program to copy “fileIN.txt” to a new file “fileOUT.txt”  
;   3- Run the code and take a screen shot of the output
;   4- In one file, include your code and the screen shot of the run                 
section .bss
	; Reserve variable data here.


section .data
	; Define variables here.                    ;  0   1  2   3   4   5   6
    array1:     dd  12, 16, 6, 18, 10, 40, 30   ;{12, 16, 6, 18, 10, 40, 30}

    maxVal:     dd  0
    minVal:     dd  0

    loopCount:  dd  0

    msg1:     db "array's element values: ",10,0
    msgL1:    equ $-msg1

    msg2:     db "array's largest value: ",10,0
    msgL2:    equ $-msg2
    
    msg3:     db "array's smallest value: ",10,0
    msgL3:    equ $-msg3

; Code goes in the text section
section .text
	GLOBAL main

main:   
    ; Build the stack.
	push	ebp
	mov		ebp, esp

    ; Print message 1
    mov     ecx, msg1
    mov     edx, msgL1
    call    PrintString


    ; Save array base address in ebx
    ; Save size of array in ecx
    mov     ebx, array1
    mov     ecx, 7

    ;; Print the array elements
    call    PrintArray

    ; Print some new lines and print the second message.
    call    Println

    ; Print message 2
    mov     ecx, msg2
    mov     edx, msgL2
    call    PrintString

    ; Restore the array address and its size to the stack before calling the multiply function.
    mov     ebx, array1
    mov     edx, 7

    call    MaxValue

    ; Restore the array address and its size to the stack before calling PrintArray function
    mov     ebx, maxVal
    mov     ecx, 1
    call    PrintArray

    ; Print some new lines and print the second message.
    call    Println

    ; Print message 2
    mov     ecx, msg3
    mov     edx, msgL3
    call    PrintString

    ; Restore the array address and its size to the stack before calling the multiply function.
    mov     ebx, array1
    mov     edx, 7

    call    MinValue

    ; Restore the array address and its size to the stack before calling PrintArray function
    mov     ebx, minVal
    mov     ecx, 1
    call    PrintArray


    ; Destroy the stack and exit the program
    mov     esp, ebp
    pop     ebp
    ret
    


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintArray:
    section .text
        push    ebp
        mov     ebp, esp
Top:
        mov     eax, [ebx]
        call    PrintDec
        call    Println
        add     ebx, 4
        loop    Top

        mov     esp, ebp
        pop     ebp
        ret


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

MaxValue:
    section .text
        pusha
        lea     eax, maxVal
        lea     eax, loopCount

Top1:
        mov     eax, [ebx]          ; Move the element of [ebx] into eax
        add     ebx, 4
        
        cmp     eax, dword[maxVal]  ; Compare it to the previously-stored value
        jg      StoreMaxValue

        inc     dword[loopCount]
        cmp     dword[loopCount], 7
        jle     Top1

        mov     dword[loopCount], 0
        popa
        ret

StoreMaxValue:
    mov     dword[maxVal], eax
    inc     dword[loopCount]
    jmp     Top1




MinValue:
    section .text
        pusha
        lea     eax, minVal
        lea     eax, loopCount
        ; Store the first value of the array into minVal
        mov     eax, [ebx]          
        mov     dword[minVal], eax
Top2:
        mov     eax, [ebx]          ; Move the element of [ebx] into eax
        add     ebx, 4
        
        cmp     eax, dword[minVal]  ; Compare it to the previously-stored value
        jl      StoreMinValue

        inc     dword[loopCount]
        cmp     dword[loopCount], 7
        jle     Top2

        mov     dword[loopCount], 0
        popa
        ret


StoreMinValue:
    mov     dword[minVal], eax
    inc     dword[loopCount]
    jmp     Top2
