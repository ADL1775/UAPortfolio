; INSTRUCTIONS/OBJECTIVE:
;   Prompt user for two integers
;   Compare the integers entered by the user
;   Display the integers in order from smallest to largest, or...
;   ...if the integers are equal, state that the two integers...
;   ...are the same value.
; INITIAL NOTES/METHOD:
;   Use exercise 2_4 as reference for _scanf call.
;   Use exercise 3_1 and 3_2 as reference for comparing two values.
;	Step 1: Get user input
;   Step 2: Use cmp instruction to set conditions for jmp instruction to desired printf call.
;			and to establish variables for output message if necessary.
;			je	= jump if equal
;			jne = jump if not equal		
;			jnz = jump if zero
;			jl  = jump if less
;			jg  = jump if greater
;	Step 3: Jump to desired output.
;	Step 4: Exit program.

global  _main
extern  _printf
extern  _scanf
extern  _ExitProcess@4

section .bss
	int1:			resd 1	; Reserve one byte for first user input
	int2:			resd 1	; Reserve one byte for second user input

section .data
	; User prompts
	prompt1:    db 'You are going to compare two integers. Please enter your first integer: ', 0ah,0
	prompt2:    db 'Enter your second integer: ',0ah,0

	; _printf statements for the two different outcomes.
	outputLg:	db 'The integers in order from smallest to largest are %d and %d.', 0ah,0
	outputEq:   db 'Your two integers are equal.', 0ah,0
		
	frmt:    db '%d',0		; Format to store user inputs.

section .text
_main:
		; Establish stack frame
		; Since _main function has been called, this is the function we are using...
		; ...we know there are 2 parameters passed (argc and argv).
		; The stack frame will be several functions deep at this point.
		push    ebp             ; Save old stack base pointer (ebp = stack base pointer RE: Week 4 Lecture).
		mov     ebp, esp        ; Make top of stack the new bottom of stack (esp = stack pointer RE: Week 4 Lecture).
								; EBP and ESP now point to the same address.

		lea     ebx, [ebp + 8]		; Load effective address of EBP + 8
		mov     ebx, [ebx]			; De-reference EBX.
		

		; Prompt the user first time and get value.
		push	prompt1		; prompt1 = "You are going to compare...enter first int..."
		call	_printf		; print prompt1
		add		esp, 4		; Add 4 bytes back to stack

		; User input for first integer.
		push	int1		; Push int1 variable.
		push	frmt		; Push frmt .data for storage of user data.
		call	_scanf		; Call scanf function for user input.
		add		esp, 8		; Add 8 bytes back to stack.

		; Prompt the user second time to get value
		push	prompt2		; prompt2 = "Enter your second integer."
		call	_printf		; Print prompt2
		add		esp, 4		; Add 4 bytes back to stack.

		; User input for second integer
		push	int2		; Push int2 variable.
		push	frmt		; Push frmt .data for storage of user data.
		call	_scanf		; Call scanf function for user input.
		add		esp, 8		; add 8 bytes back to stack
							; Stacks now pointing to same place
		

		; Prepare registers.
		xor		eax, eax		; For int1
		xor		edx, edx		; For int2.

		; EAX is now where I want to work with the FIRST user integer
		; EDX is now where I want to work with the SECOND user integer


		; COMPARE THE INTEGERS

		mov		eax, dword [int1]	; Move int1 value into EAX
		mov		edx, dword [int2]	; Move int2 value into EDX

		cmp		eax, edx			; Compare EAX with EDX
		jg		greaterPush			; Jump to greater: if EAX (int1) > EDX (int2)
		jl		lesserPush			; Jump to lesser: if EAX (int1) < EDX (int2)
		jmp		finishEq			; Jump to finishEq: if above conditions not met, meaning both integers are equal.


;If int1 > int2...
greaterPush: 
		push	eax				; Push int1 onto stack first because it is bigger and must be displayed last.
		push	edx				; Push int2 onto stack second because it is smaller and must be displayed first.
		jmp		finishLg		; Jump to finishLg to call printf statement stating which integer is larger.

; If int1 < int2...
lesserPush:
		push	edx				; Push int2 onto stack first because it is bigger and must be displayed last.
		push	eax				; Push int1 onto stack second because it is smaller and must be displayed first.
		jmp		finishLg		; Jump to finishLg to call printf statement stating which integer is larger.

finishLg:
		push	outputLg		; Push output string if integers are different.
		call	_printf			; Print the statement.
		add		esp, 12			; Add 12 bytes back to stack (to account for 3 push commands).
		jmp		end				; Jump to end:

finishEq:
		push	outputEq		; Push output string if integers are equal.
		call	_printf			; Print the statement.
		add		esp, 4			; Add 4 bytes back to stack.

end:
		; Re-Establish Stack Frame
		;mov     esp, ebp        ; Move base pointer back to top stack.
		;pop     ebp             ; Pop base pointer (take off stack and putting back into EBP).


		; Exit the program.
		push    0
		call    _ExitProcess@4