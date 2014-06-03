; -----------------------------------------------------------------------------
; A 64-bit app
;
;  nasm  -felf64 string.asm
; 
; -----------------------------------------------------------------------------

        global  main
        extern  puts

section .text
main:
        push    r12                     ; save callee-save registers
        push    r13
        push    r14
        ; By pushing 3 registers our stack is already aligned for calls
        mov     edi, hello

        call    puts
        ; restore save
        mov     r8, hello
boucle:        
        cmp     byte [r8], 0                  ; check for end of string
        jz      endloop
        xor     byte [r8], 0x01
        inc     r8
        jmp     boucle
endloop:
        mov     edi, hello
        call    puts

        mov     r8, hello
        jmp     boucle
        ; restore saved registers
        pop     r14
        pop     r13
        pop     r12
        ret

section .data
hello:
        db      "Hello world", 0
index:
        dw      0