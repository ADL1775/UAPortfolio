; In this exercise, write an assembly language function to find the largest string in a list of strings.
; The function has two parameters. First is a pointer to a list of strings.
; Second is a count of the number of strings in the list. 
; In C, prototype function is as follows: 'char *FindLongestString(char *pStringList[], int stringCount);'
; You can assume the list parameter is an array of pointers to the strings that make up the list.
; Function must return a pointer to the largest string in the list.
;
; Objective:    Find find the largest string in a list of strings.
; Method:       Get address of string and push as argument into a register through function call./
;               Pull one character at a time off the string until null-terminating byte is found using a loop.
;               Keep count of how many times that loop has gone through string. /
;               Once null-terminating byte is found, end of string is reached. /
;               Store count of loop (string) in a register value. /
;               Compare other string lengths to that register value. /
;               Replace register value with longer string, but not shorter string. /
;               SIMULTANEOUSLY
;               Determine number of strings and store in register value.
;               Once number of strings determined, go to next string and decrease register value by 1.
;               Once string count reaches 0, return memory address of longest string.


global  _FindLongestString  ; This is the Assembly function (under section .text) that the C program can access.
global  _main
extern  _strlen
extern  _ExitProcess@4

section .bss
    ; Uninitialized variables. Examples: 
    ; buffer: resb 1  reserve 1 byte in variable "buffer"
    ; buffer1: resd 1 reserve 1 double-byte in varialbe "buffer1"

    longestString:  resd 1    ; always points to the longest string
    longestLength:  resd 1    ; always holds the length of the longest string
    count:          resd 1    ; Remaining strings

section .data


section .text
_FindLongestString:

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
    push    ebp             ; Save old stack base pointer (ebp = stack base pointer RE: Week 4 Lecture).
    mov     ebp, esp        ; Make top of stack the new bottom of stack (esp = stack pointer RE: Week 4 Lecture).
                            ; EBP and ESP now point to the same address.
    
    ; mov     EDX, EBP + 12 
    ; add     EDX, 12
    ; EBP is currently pointing to top of stack.
    ; We want EDX to point to address that contains the number of strings.
    ; EBP + 8 would point to first argument passed on line 44 in C program...
    ; ...FindLongestString(availableStrings). This points to address of the list of strings.
    ; 4 bytes from that address is # of strings, the second argument passed. 
   
    lea     edx, [ebp + 12] ; LEA stands for Load Effective Address.
                            ; Moves the effective address of string into EDX. Equivalent of top two comments above.
                            ; EDX now points to the parameter in stack.
                            ; Loads address 8 bytes greater than ebp into ebx since ebp + esp = 8 bytes. 
                            ; This points to the address of the argument we want to access (in this case, the start of the string).
                            ; This happens because the argument in the C program of 'mystrlen' begins, the function of this program.
                            ; 'mystrlen' in C has argument of 'teststring' passed into it.
                            ; 'teststring' is the string we want to count the characters of.
                            ; Could use any register (eax, edx, etc.). It's up to you. Pick one and keep track of it.
   
    mov     edx, [edx]      ; Move the address that EDX is pointing to into EDX (aka dereference).
                            ; Instead of pointing to the stack, it gets value on stack and points to list of strings.

   
    mov     [count], edx    ; # of strings via ebp+12 --> edx --> [count] now stored in variable [count].
    
    
    ;Re-Establish stack frame to address of first string in array.
    lea     ebx, [ebp + 8]  
   
    mov     ebx, [ebx]      ; Move the address that EBX is pointing to into EBX (aka dereference).
                            ; Instead of pointing to the stack, it gets value on stack and points to list of strings.
                            ; EBX will be pointing to first character array in list of availableStrings.
                            ; NOTE: This is pointing to 'char *availableStrings[6]', which is a pointer.
                            ;       Essentially, we created a pointer to a pointer.


    ; Zero out working and string counter register.
    xor     eax, eax        ; Working register.

    ; EAX is now where I want to work with data and is currently zeroed.
    
    
    ; Initialize the longestLength variable from section.bss
    mov     [longestLength], dword 0    ; Use longestLength to track the longest string.


; Make the loop to determine longest string.
startOfLoop:
  
    ; PURPOSE: Get String Length. We are going to call _strlen which takes...
    ; ...a parameter pointer to pavailableStrings[i] on line 66 in C program.
    ; Since we had EBX point to the address of that first string...
    ; ...we need to push the first value in that string.
    push    ebx             ; Push EBX onto stack.
    push    dword [ebx]     ; Get first pointer value in array that EBX is pointing to.
    call    _strlen         ; Get the string length using _strlen function from C program.
                            ; This eliminates the need to manually count characters in this assembly program like in exercise 3_1.

    add     esp, 4          ; Restore stack
    pop     ebx             ; Since we are using EBX as a register, we want to preserve EBX across the call to strlen.

    ; EBX contains the ADDRESS of the POINTER to the first string.

    ; Compare result of strlen with current longest length.
    cmp     eax, [longestLength]    ; Since return values are passed through EAX. 
    jl      finish                  ; Jump to 'finish:' if EAX is less than longestLength. 
                                    ; EAX has a value due to strlen function above, meaning a jump of "if less than or equal"...
                                    ; ...is not required.

    ; If EAX is greater than current [longestLength]...
    ; ...save EAX the new longestLength address. 
    mov     [longestLength], eax
    
    ; Then we save pointer to longest string.
    mov     eax, dword [ebx]        ; Move EBX pointer into EAX. This is the current longestString that was determined
                                    ; through the compare operation.
    mov     [longestString], eax    ; Replace longestString pointer with current EAX value, the new current longestString.


finish:
    ; PURPOSE: Move to next list entry.
    ; This process begins after [longestLength] is compared to and determined to be LESS THAN existing EAX value.
    ; It then moves to the next address in memory. This will also be the next string in the list.
    add     ebx, 4

    ; See if we reached the end of the list of strings.
    dec     byte [count]         ; Decrement count located in [count] variable. 
    jnz     startOfLoop          ; Jump to startOfLoop if counter is =/= 0.

    ; Replace whatever is in EAX to [longestString].
    ; Prior to this, EAX currently has the string length return value if the last string in array was not the longest string.
    mov     eax, dword [longestString]
    

    ; PURPOSE: Remove stack frame.
    ; If we just ret here, the program will act on the next instruction in the stack, which is EIP.
    ; Therefore, the stack is now misaligned. 
    mov     esp, ebp        ; Move base pointer back to top stack.
    pop     ebp             ; Pop base pointer (take off stack and putting back into EBP).
                            ; Now it points to address it needs to go to in order to continue program.
    ret