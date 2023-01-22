; Lab12_2.asm
;   1- Create a file “file1.txt” that has your name
;   2- Using system calls, write assembly program to copy “fileIN.txt” to a new file “fileOUT.txt”  
;   3- Run the code and take a screen shot of the output
;   4- In one file, include your code and the screen shot of the run                 
section .bss
	; Reserve variable data here.
    fd_in:      resd 1          ; Store file descriptor for input file
    fd_out:     resd 1          ; Store file descriptor for output file

section .data
	; Define variables here.    
    openFile:	    db  "fileIN.txt",0
	writeFile:	    db  "fileOUT.txt",0
    name TIMES 64   db  0



; Code goes in the text section
section .text
	GLOBAL main

main:   
    ; Build the stack.
	push	ebp
	mov		ebp, esp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Open the file for reading | sys_open
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;   Put the system call sys_open() number 5, in the EAX register.
    ;   Put the filename in the EBX register.
    ;   Put the file access mode in the ECX register.
    ;   Put the file permissions in the EDX register
    ;
    ;   The system call returns the file descriptor of the created file in the EAX register
    ;   In case of error, the error code is in the EAX register.

    mov     ebx, openFile   ; Give EBX the file name
    mov     ecx, 0x0          ; For read-only access
    mov     edx, 0x177      ; Read, write, execute by all
    mov     eax, 0x5          ; sys_open call where eax = 5

    int     80h

    mov     [fd_in], eax    ; Save file descriptor


    mov     eax, 0x5        ; sys_open
    mov     ebx, writeFile  ; File name
    mov     ecx, 0x41       ; File mode: create
    mov     edx, 0x177      ; Permissions if file does not exist
    int     80h

    mov     [fd_out], eax   ; Save file descriptor

ReadFileStart:        
    mov     eax, 3
    mov     ebx, [fd_in]
    mov     ecx, name
    mov     edx, dword 25
    int     80h

    mov     ebx, 0
    cmp     ebx, eax
    je      ReadFileEnd     ; End of File (EOF) reached

    ; Now we can write to the file
    mov     ebx, [fd_out]   ; Move filed descriptor into ebx    
    mov     ecx, name
    mov     dword edx, eax  ; Number of bytes from input file
    mov     eax, 4          ; sys_write
    int     80h             ; Call kernel

    mov     ebx, 25         ; If less than 25 bytes is written, we are at the EOF.
    cmp     ebx, eax

    jb      ReadFileEnd
    jmp     ReadFileStart



ReadFileEnd:
    ; CLOSE THE FILES
    mov     eax, 6
    mov     ebx, [fd_in]
    int     80h

    mov     eax, 6
    mov     ebx, [fd_out]
    int     80h

    ; CLOSE THE PROGRAM
    mov     esp, ebp
    pop     ebp
    ret
    