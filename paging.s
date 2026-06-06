global paging_init

section .data
align 4096
page_directory:
    times 1024 dd 0

section .text
paging_init:
    ; first 4MB: present + RW + user + 4MB page = 0x87
    mov dword [page_directory], 0x00000087

    mov eax, page_directory
    mov cr3, eax

    mov eax, cr4
    or  eax, 0x00000010     ; set PSE (4MB pages)
    mov cr4, eax

    mov eax, cr0
    or  eax, 0x80000000     ; set PG (enable paging)
    mov cr0, eax

    ret

section .note.GNU-stack noalloc noexec nowrite progbits
