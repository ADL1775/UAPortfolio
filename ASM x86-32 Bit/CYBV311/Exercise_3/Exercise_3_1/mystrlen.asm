; Write in assembly the equivalent function to the strlen(). This is passed through argument 'int strlen(char *string);'
; Function must be callable from a C program.


; Objective:    Find length of string from Exercise3.c and print value to screen.
; Method:       Get address of string and push as argument into a register through function call.
;               Pull one character at a time off the string until null-terminating byte is found using a loop.
;               Keep count of how many times that loop has gone through string. 
;               Once null-terminating byte is found, end of string is reached.
;               Count of loop will then = strlen.

[BITS 32]

global  _mystrlen       ; This is the Assembly function (under section .text) that the C program can access.
extern  _printf
extern  _scanf
extern  _ExitProcess@4

section .bss
char:       resd 1

section .data


section .text
_mystrlen:      ; This is the function that will be called into the .c program indicated by 
                ; "extern int mystrlen(char* string);" in c
    
    ; Establish stack frame. 
    ; This will ensure that the base pointer will be at a certain point in reference to other information in stack.
    ; In other words, it establishes where the new function begins and will reference items based off of this point.
    ; If this isn't done, it is hard to ensure where certain items are in the stack.
    ; We already know there are certain things on the stack: Arguments pushed before the call (in this case, mystrlen in c program).
    ; Once the function is called, EIP will be pushed onto stack from the call. 
    ; EIP is an address in memory. It tells the program where to go next to execute the next command and 
    ; controls the flow of a program. Directly after that address is where we want to go.
    ; Shorthand:
    ; ESP --> EIP (Retrun Location) from call
    ;     --> Arguments pushed before call
    ; 
    push    ebp             ; Save old stack base pointer (ebp = stack base pointer RE: Week 4 Lecture).
    mov     ebp, esp        ; Make top of stack the new bottom of stack (esp = stack pointer RE: Week 4 Lecture).
                            ; EBP and ESP now point to the same address.
    
    ; mov     ebx, ebp + 8 
    ; add     ebx, 8
    lea     ebx, [ebp + 8]  ; LEA stands for Load Effective Address.
                            ; Moves the effective address of string into EBX. Equivalent of two comments above.
                            ; EBX now points to the parameter in stack.
                            ; Loads address 8 bytes greater than EBP into EBX since EBP + ESP = 8 bytes. 
                            ; This points to the address of the argument we want to access (in this case, the start of the string).
                            ; This happens because the argument in the C program of 'mystrlen' begins, the function of this program.
                            ; 'mystrlen' in C has argument of 'teststring' passed into it.
                            ; 'teststring' is the string we want to count the characters of and where the stack pointer is now located.
                            ; Could use any register (eax, edx, etc.). It's up to you. Pick one and keep track of it.
    
    mov     ebx, [ebx]      ; Move the address that ebx is pointing to into ebx.
                            ; Instead of pointing to the stack, it gets value on stack and points to list.
                            ; ebx will be pointing to first entry in list ("This is a test string";)
    
    ; Zero out working and counter register.
    xor     eax, eax        ; Working register.
    xor     ecx, ecx        ; Counter register.

    ; EAX is now where I want to work with data and is currently zeroed.
    ; ECX is now where I want to count the number of characters in the string and is currently zeroed.
    ; EBX contains the ADDRESS of the first letter of the string.

; Make the loop to count characters in the string until null-terminating byte is reached.
startOfLoop: 
    mov     eax, [ebx]      ; Move de-referenced address of ebx into eax. (MIGHT NEED TO ADD dword BEFORE EBX).
                            ; Will copy first 4 bytes of teststring into eax.
                            ; We don't want to look at 4 letters at a time. 
                            ; Need to strip out other data and compare first letter to value of 0 (the null byte).

    and     eax, 0xFF       ; Performs AND operation on working data AND 0xFF.
                            ; 0xFF indicates "on".
                            ; Strips out the first three bytes and will turn on the last (least significant) byte.
                            ; This will map to EAX.
                            ; Takes 4 characters and returns 1 character back.
                            ; Any bits that are "on" at the end will remain "on".
                            ; If null byte is operated on (end of the string), it will recognize "off".
                            
    cmp     eax, 0          ; If EAX equals 0, then...
    je      finish          ; Jump to the 'finish' function below this loop function.
                            ; If EAX does NOT equal 0, set conditions below.
    inc     ecx             ; Increase ECX by 1. REMEMBER: This is the established counter register.
    inc     ebx             ; Increase EBX by 1. REMEMBER: This is the address to the start of the string. 
                            ; This moves to next character in string and points to it.
    jmp     startOfLoop     ; Go back to the start of loop and repeat. 
                            
    ; We are now cycling through the characters in the string!
    ; Note: ECX register value in disassembly will represent the count in a hex value. 
    ;       In 'teststring', the character count = 21 (decimal) which = 15 in hex.
    ;       Therefore ECX register value equals 15 but the character count is 21. 


finish:
    ; Our count is currently in ECX register. We need it in EAX.
    ; Move value of counter register (ECX) into working data (EAX).
    ; When ret is called, this value will be outputted into _mystrlen
    mov     eax, ecx        
    
    ; Remove stack frame.
    ; If we just ret here, the program will act on the next instruction in the stack, which is EIP.
    ; Therefore, the stack is now misaligned. 
    mov     esp, ebp        ; Move base pointer back to top stack.
    pop     ebp             ; Pop base pointer (take off stack and putting back into EBP).
                            ; Now it points to address it needs to go to in order to continue program.

    ret