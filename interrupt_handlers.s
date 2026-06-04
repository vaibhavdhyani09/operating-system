extern interrupt_handler

%macro no_error_code_interrupt_handler 1
    global interrupt_handler_%1
interrupt_handler_%1:
    push    dword 0
    push    dword %1
    jmp     common_interrupt_handler
%endmacro

common_interrupt_handler:
    pusha
    mov     eax, [esp + 32]
    push    eax
    call    interrupt_handler
    add     esp, 4
    popa
    add     esp, 8
    iret

    no_error_code_interrupt_handler 0
    no_error_code_interrupt_handler 1
    no_error_code_interrupt_handler 33
