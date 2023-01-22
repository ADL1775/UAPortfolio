
; OBJECTIVE: Print contents of the structure from C program in the following format:
;            First Name
;            Last Name
;            Class
;            Student ID.


global _PrintRecord
extern _printf

section .bss

section .data
    ; Since we want to output the structure in a specific format, declare variables.
    formatStr: db 'First Name:  %s', 0ah,'Last Name:   %s', 0ah,'Class:       %d', 0ah,'Student ID:  %d', 0ah,0

section .text
_PrintRecord:
    
    ; Right now, we are located at line 41 in the C program since...
    ; ..._PrintRecord here in assembly is the function being called by C.
    ; This is like using a scalpel to manage what the function does.


    ; Build the stack
    push    ebp             
    mov     ebp, esp            ; Setup new stack frame. EBP (base pointer) is starting point. ESP moves. 
                                ; Right now, ESP and EBP are at same location.
                              
    
    ; EBP is not going to move in this function.
    ; We do not want to move EBP in the function.
    ; PrintRecord on line 41 in C has 1 argument passed into it (the address of 'record' on line 27).
    ; Right now, ESP and EBP are at same location.
    ; EBP + 0 = Old EBP
    ; EBP + 4 = Return Instruction in C
    ; EBP + 8 = Argument in C

    mov     edx, [ebp + 8]      ; Moving address of structure into EDX.

    ; Since EDX points to the very beginning of structure...
    ; ...we want it to point relative to the end of the structure so we can push those arguments.
    ; EDX is currently the value of the structure.
    ; Size of structure:
    ;   FName:          64
    ;   MName:          64
    ;   LName:          64
    ;   Class int;      4
    ;   StudentID int:  4
    ;   Total bytes:    200

    add     edx, 200            ; Now we are at end of structure.
                                ; Or, since we know StudentID is a 4-byte integer,...
                                ; ...we could just add 196 to get to the beginning of that part of the structure.

    ; Get the data from structure.
    sub     edx, 4              ; Back up 4 bytes. Now at beginning of studentID value since int is 4 bytes.
    push    dword [edx]         ; De-reference the value at EDX and the data is now pushed onto stack.

    sub     edx, 4              ; Beginning of Class ID address.
    push    dword [edx]         ; De-reference the value at EDX and the data is now pushed onto stack.

    sub     edx, 64             ; Beginning of LName.
    push    edx                 ; Push the address of LName.

    sub     edx, 128            ; Beginning of FName. This skips over MName per instructions.
                                ; If you wanted to get the MName, the just sub 64, then add another iteration below of sub 64 to get FName.
    push    edx                 ; Push the address of FName.

    push    formatStr           ; Push Format String.

    call    _printf             ; printf the Format String.
    

    ; Remove stack frame
    mov      esp, ebp        ; Base pointer back to top of stack.
    pop      ebp             ; Pop matches push under 'Build the stack'.

    mov eax, 0 ; return NULL

    ret