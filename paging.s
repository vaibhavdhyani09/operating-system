global paging_init

section .data
align 4096
page_directory:
    times 1024 dd 0

section .text
paging_init:
    mov dword [page_directory], 0x00000083

    mov eax, page_directory
    mov cr3, eax

    mov eax, cr4
    or  eax, 0x00000010
    mov cr4, eax

    mov eax, cr0
    or  eax, 0x80000000
    mov cr0, eax

    ret
