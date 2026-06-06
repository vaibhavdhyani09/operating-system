#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "multiboot.h"
#include "io.h"
#include "paging.h"
#include "pmm.h"

static void serial_write_str(const char *str)
{
    while (*str) {
        while (!(inb(0x3F8 + 5) & 0x20));
        outb(0x3F8, *str++);
    }
}

/* simple hex printer for serial, to verify addresses */
static void serial_write_hex(unsigned int n)
{
    char buf[11];
    int i;
    buf[0] = '0'; buf[1] = 'x';
    for (i = 9; i >= 2; i--) {
        int nibble = n & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('a' + nibble - 10);
        n >>= 4;
    }
    buf[10] = '\0';
    serial_write_str(buf);
}

/* kernel_end is exported by the linker script */
extern unsigned int kernel_end;

void kmain(unsigned int ebx)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;

    fb_clear();
    gdt_init();
    pic_remap();
    idt_init();
    paging_init();

    /* Chapter 10: init page frame allocator */
    unsigned int kernel_phys_end = (unsigned int) &kernel_end;
    pmm_init(mbinfo->mem_upper, kernel_phys_end);

    serial_write_str("paging enabled\n");
    serial_write_str("pmm initialised\n");

    /* test: allocate 3 frames and print their addresses */
    unsigned int f1 = pmm_alloc();
    unsigned int f2 = pmm_alloc();
    unsigned int f3 = pmm_alloc();

    serial_write_str("frame1: "); serial_write_hex(f1); serial_write_str("\n");
    serial_write_str("frame2: "); serial_write_hex(f2); serial_write_str("\n");
    serial_write_str("frame3: "); serial_write_hex(f3); serial_write_str("\n");

    /* free frame2 and reallocate - should get same address back */
    pmm_free(f2);
    unsigned int f2b = pmm_alloc();
    serial_write_str("frame2 after free+alloc: ");
    serial_write_hex(f2b);
    serial_write_str("\n");

    serial_write_str("kmain started\n");

    if (mbinfo->mods_count != 1) {
        serial_write_str("ERROR: module not loaded\n");
        fb_write("ERROR: module not loaded!", 25, FB_WHITE, FB_BLACK);
        __asm__ __volatile__("sti");
        while (1) {}
        return;
    }

    serial_write_str("module found, jumping\n");
    fb_write("PMM OK. Module found! Jumping...", 32, FB_WHITE, FB_BLACK);
    fb_write("  |  Type below:", 16, FB_WHITE, FB_BLACK);

    __asm__ __volatile__("sti");

    unsigned int *mods = (unsigned int *) mbinfo->mods_addr;
    unsigned int program_addr = mods[0];

    typedef void (*call_module_t)(void);
    call_module_t start_program = (call_module_t) program_addr;
    start_program();

    while (1) {}
}
