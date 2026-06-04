#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "fb.h"

void kmain(void)
{
    fb_clear();
    fb_write("Type something: ", 16, FB_WHITE, FB_BLACK);
    gdt_init();
    idt_init();
    pic_remap();
    __asm__ volatile ("sti");
    for (;;) { __asm__ volatile ("hlt"); }
}
