extern interrupt_handler

%macro no_error_code_interrupt_handler 1
    global interrupt_handler_%1
interrupt_handler_%1:
    push    dword 0
    push    dword %1
    jmp     common_interrupt_handler
%endmacro

common_interrupt_handler:
    pusha                   ; save all general registers (32 bytes)

    mov     eax, [esp + 32] ; interrupt number (skip the 8 saved regs)
    push    eax             ; single argument: interrupt number
    call    interrupt_handler
    add     esp, 4          ; pop the argument

    popa                    ; restore registers
    add     esp, 8          ; remove interrupt number + error code
    iret

    no_error_code_interrupt_handler 0
    no_error_code_interrupt_handler 1
    no_error_code_interrupt_handler 33
section .note.GNU-stack noalloc noexec nowrite progbits
