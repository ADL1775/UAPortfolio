; INSTRUCTIONS/OBJECTIVE:
;   Change case of each character in a string.
;   Function accepts pointer to target string (int SwapCase(char *string);)
;	Convert lowercase character to uppercase, and vice versa.
;
; INITIAL NOTES/METHOD:
;   Use exercise 2_4 as reference for _scanf call.
;	Use exercise 3_1 as reference for counting a strlen.
;   Use exercise 3_3 as reference for pointer locations.
;	Step 1:		User input stored in char inputString[32] in C Program
;	Step 2:		Find where inputString is stored in reference to current stack position.
;	Step 3a:	Convert case of each character in the string.
;	Step 3b:	Special characters and spaces remain the same.
;	Step 4:		Return the case-swapped sting.
;
;	
;CONVERSIONS (Step 3a):
;	On ASCII table, decimal values for lowercase letters are..
;		...32 bytes above their uppercase values. 
;		LowerCase: Dec 97 (a) through 122 (z)
;		UpperCase: Dec 65 (A) through 90 (Z)
;			     sub = 32			  32
;
;	THEREFORE: 
;		Lowercase letters need decimal value 32 subtracted to get uppercase.
;		Uppercase letters need decimal value 32 added to get lowercase.



; IMPORTANT NOTE: Tried making variable [newStr] as placeholder for new inputString and moving into EAX before ret, but...
;
; Once ret is executed, instructions follow from C program:
; add	esp, 4
; lea	eax, [inputString]					; [inputString] ALREADY HAS A PLACEHOLDER IN MEMORY!
; push	eax
; push	"Your string after SwapCase..."
; call	_printf
;
; THEREFORE: SwapCase function must convert [inputString] for output since it will be passed into EAX AFTER ret!!!!!


[BITS 32]
global _main
global _SwapCase	; This is the Assembly function (under section .text) that the C program will call on line 32.
extern  _ExitProcess@4

section .bss
	count:			resb 1		; Use to count remaining chars from user input string.
	asciiOne:		resd 1		; Use to compare current value and what to do with it for conversion. Explanation below under _SwapCase
	asciiTwo:		resd 1		; Use to compare current value and what to do with it for conversion. Explanation below under _SwapCase
	asciiThree:		resd 1		; Use to compare current value and what to do with it for conversion. Explanation below under _SwapCase
	asciiFour:		resd 1		; Use to compare current value and what to do with it for conversion. Explanation below under _SwapCase

section .data


section .text
_SwapCase:
		
	; PREPARATION

		; _SwapCase points to line 32 of C program
		; Establish stack frame to access argument passed into it (inputString).
		push    ebp					; Save old stack base pointer (ebp = stack base pointer RE: Week 4 Lecture).
		mov     ebp, esp			; Make top of stack the new bottom of stack (esp = stack pointer RE: Week 4 Lecture).
									; EBP and ESP now point to the same address.
		
		; EBP is currently pointing to the top of the stack.
		; We want EBX to point to the user-inputted string.
		; Since _SwapCase is called on line 32 of C program, and this is the function we are on...
		; ...EBP + 8 would point to the ADDRESS first argument passed on line 32 of C program: 'SwapCase(inputString);'
		; This points to the address of the user-inputted string ([inputString]).
		; We also know that the size of 'char inputString' is 32 bytes.

		lea     ebx, [ebp + 8]		; Move address of 'char inputString[32];' into EBX.
		mov     ebx, [ebx]			; De-reference. Now EBX points to first character of user string.

		
		; Prepare registers.
		xor		eax, eax			; Working register
		xor		ecx, ecx			; Use to cmp current byte value with a null-terminator.
		xor		edx, edx			; Use to move byte values of [asciiOne-through-Four] into for comparison against EAX.

				
		; Initialize variables
		
		mov		[count], byte  32		; Since inputString is <= 32 bytes, just mov dword 32 into [count].


		mov		[asciiOne], byte 64		; Use to compare current value and what to do with it for conversion.
										; Since char A is at dec 65, anything above that will be Uppercase...
										; ...and will need 32 added to it. 
										; Anything below 64 will need nothing to happen.
		
		mov		[asciiTwo], byte 91	    ; Use to compare current value and what to do with it for conversion.
										; Since char Z is at dec 90, anything above that will be lowercase...
										; ...and will need 32 subtracted from it. 
										; Anything below 91 will need 32 added to it.
		
		mov		[asciiThree], byte 96	; Use to compare current value and what to do with it for conversion.
										; Since char a is at dec 97, anything above that will be lowercase...
										; ...and will need 32 subtracted from it.
										; Anything below that is > 90 will need nothing to happen.

		mov		[asciiFour], byte 123	; Use to compare current value and what to do with it for conversion.
										; Since char z is at dec 122, anything above it will be left alone.
										; Anything below 91 will need 32 added to it.

	; SWAP THE CASE OF CHARACTERS

; Use loop to read each byte decimal value, convert to opposite case, and move to next byte.
; End condition: EBX = 0 = null-terminator.
startOfLoop: 
		
		;;;;;;;;;; AT THIS POINT: EBP = ESP = [EBX] (convert EBP and ESP endianness to get EBX register value)


		push	ebx						; Push EBX onto stack. EBX contains character that needs to be converted.
		push	dword [ebx]				; Get value of character pointed to by EBX.

		movzx	eax, byte [ebx]			; Move first character into EAX.

		movzx	edx, byte [asciiOne]	; Move decimal value of asciiOne into EDX
		cmp		eax, edx				; Determine if EAX is less than decimal value 64
		jle		finishChar				; If EAX is <= dec 64, move to end to keep character.
		
		movzx	edx, byte [asciiTwo]	; Move decimal value of asciiTwo into EDX
		cmp		eax, edx				; Determine if EAX is less than decimal value 91
		jl		addByte					; If EAX is < dec 91 it is uppercase and needs to change to lowercase.

		movzx	edx, byte [asciiThree]	; Move decimal value of asciiThree into EDX
		cmp		eax, edx				; Determine if EAX is less than decimal value 96
		jle		finishChar				; If EAX is <= dec 96, move to end to keep character.

		movzx	edx, byte [asciiFour]	; Move decimal value of asciiFour into EDX
		cmp		eax, edx				; Determine if EAX is less than decimal value 123
		jl		subByte					; If EAX is < dec 123, it is lowercase and needs to change to uppercase.
		jge		finishChar				; If EAX is >= dec 123, move to end to keep character.


addByte: 
	add		eax, 32						; Add 32 bytes to EAX to get equivalent lowercase character.
	jmp		finishChar					; Jump to finishChar

subByte: 
	sub		eax, 32						; Subtract 32 bytes to EAX to get equivalent uppercase character.
	jmp		finishChar					; Jump to finishChar

finishChar: 
	; PURPOSE AND STEPS:
	;	1. Move the converted character stored in EAX into [EBX] (which will part of the final output into C program).
	;	2. Move to next character in user string.
	;	3a. Count down remaining characters.
	;	3b.If no characters remain, move EBX into EAX in order to...
	;	...be outputted in C program as the swapCase string.

	
	mov		byte [ebx], al		; Move lowest-order byte of EAX (the changed decimal value) into [EBX].
								; ATTEMPTED OPERATIONS:
								; mov		[ebx], eax				; Replaces a 4-byte value from EAX into EBX (xx 00 00 00).
								; movzx		byte [ebx], eax			; Run failure (invalid operation)
								; movzx		ebx, byte [eax]			; Crash
								; mov		ebx, byte [eax]			; Run failure (invalid operation)
	
	inc		ebx					; Increase EBX by 1 byte to move to next character.
	
	xor		edx, edx			; Zero out EDX register to await new decimal value for comparison to EAX.
	

	; Determine if end-of-string is reached indicated by null-terminating byte.
	; Remember: A characteristic of a string in C is that it ends with a null-terminating byte. 

	movzx	ecx, byte [ebx]	; Move the current byte value of EBX into ECX.
	cmp		ecx, 0			; See if current EBX byte value (now ECX) is equal to 0.
	jne		startOfLoop		; Jump to startOfLoop if byte value =/= 0.
							; If byte value = 0, then the string is complete.
							; REMEMBER: Spaces are NOT bytes. They are decimal value 20 (device control 4).
	
	; Once current byte value of EBX = 0, we have converted the characters of [inputString].
	; If we continue, then the C program will ouput the first 32 characters of [inputString], then...
	; ...output remainder in another two printf statements.


	; PROGRAM END IF NULL-TERMINATOR IS FOUND.
	; REMINDER: AFTER ret is executed at end of this program, address of inputString, where EBP is pointing to,...
	;			...will be moved into EAX. Therefore the following instruction is not needed:	
	; mov		eax, ebx		; This typically moves whatever was in EBX into EAX for output.
								; But this is done after ret with instruction "lea	eax, [inputString]" inherent to the C printf call.
	

    ; PURPOSE: Remove stack frame.
    ; If we just ret here, the program will act on the next instruction in the stack, which is EIP.
    ; Therefore, the stack is misaligned. 
	
	mov     esp, ebp        ; Move base pointer back to top stack.
    pop     ebp             ; Pop base pointer (take off stack and putting back into EBP).
                            ; Now it points to address it needs to go to in order to continue program.
	
	
	ret

	; Once ret is executed, instructions follow from C program:
	; add	esp, 4
	; lea	eax, [inputString]					; [inputString] ALREADY HAS A PLACEHOLDER IN MEMORY!
	; push	eax
	; push	"Your string after SwapCase..."
	; call	_printf



; NOTE FROM PROF DUREN (Email sent on 2 May 2022):
; Subject: What happens if user inputs a string > 32 bytes?
;
; In this case, inputString is a 32-byte buffer that resides on the stack of the function main().  
; As such, it resides in the memory space next to any parameters that main() has and the main function’s return address.  
; What’s important to understand is that when SwapCase is called, a pointer to inputString is passed.  
; It points to inputString’s home on the stack.  SwapCase has no idea where the memory is (e.g. on the stack) and it does not know how large the buffer is.  
; Therefore, if the string is longer than 32 bytes, SwapCase will just keep reading and potential writing beyond the inputStrings reserved bytes.  
; It’s not that the extra data cannot be found in memory, it surely can, but the data resides outside the bounds of the buffer,...
; thus potentially residing on top of other, potentially important, data that’s on the stack.