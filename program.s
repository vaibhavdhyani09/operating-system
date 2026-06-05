bits 32

global _start
_start:
    mov eax, 0xDEADBEEF
.loop:
    jmp .loop
