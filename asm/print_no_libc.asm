section .bss

  section .data
      hello: db "Hello World",0xa      ;10 is EOL

  section .text
      global _start

  _start:

      mov ecx, 0;                      ; int i = 0;
  loop:
      mov dl, byte [hello + ecx]       ; while(data[i] != EOF) {
      cmp dl, 0xa                      ;
      je  exit                         ;
      mov ebx, ecx                     ; store conetents of i (ecx)

      ; Print single character
      mov eax, 4                       ; set sys_write syscall
      mov cl, byte [hello + ebx]      ; ...
      mov edx, 1                       ; move one byte at a time
      syscall

      inc ebx                          ; i++
      mov ecx, ebx                     ; move ebx back to ecx
      jmp loop                         ;

  exit:
      mov eax, 0x01                    ; 0x01 = syscall for exit
      syscall
