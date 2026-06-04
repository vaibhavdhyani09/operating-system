#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "multiboot.h"

void kmain(unsigned int ebx)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;

    fb_clear();
    gdt_init();
    pic_remap();
    idt_init();

    if (mbinfo->mods_count != 1) {
        fb_write("ERROR: module not loaded!", 25, FB_WHITE, FB_BLACK);
        /* still keep keyboard working even on error */
        __asm__ __volatile__("sti");
        while (1) {}
        return;
    }

    fb_write("Module found! Jumping to program...", 35, FB_WHITE, FB_BLACK);
    fb_write("  |  Type below:", 16, FB_WHITE, FB_BLACK);

    /* enable interrupts so keyboard works */
    __asm__ __volatile__("sti");

    /* jump to the loaded program */
    unsigned int *mods = (unsigned int *) mbinfo->mods_addr;
    unsigned int program_addr = mods[0];

    typedef void (*call_module_t)(void);
    call_module_t start_program = (call_module_t) program_addr;
    start_program();

    /* program loops forever, but if it ever returns keep keyboard alive */
    while (1) {}
}
