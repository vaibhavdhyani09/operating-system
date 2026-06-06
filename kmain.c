#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "multiboot.h"
#include "io.h"
#include "paging.h"
#include "pmm.h"
#include "usermode.h"
#include "tss.h"

static void memcpy_local(unsigned char *dst, unsigned char *src,
                         unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
        dst[i] = src[i];
}

extern unsigned int kernel_end;

/* kernel stack — TSS esp0 points to the top of this */
static unsigned int kernel_stack[1024];

void kmain(unsigned int ebx)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;

    fb_clear();
    gdt_init();
    pic_remap();
    idt_init();
    paging_init();

    unsigned int kernel_phys_end = (unsigned int) &kernel_end;

    unsigned int *mods      = (unsigned int *) mbinfo->mods_addr;
    unsigned int prog_start = mods[0];
    unsigned int prog_end   = mods[1];
    unsigned int prog_size  = prog_end - prog_start;

    pmm_init(mbinfo->mem_upper, kernel_phys_end, prog_end);

    /* set up TSS so inter-privilege interrupts have a kernel stack */
    unsigned int kernel_esp = (unsigned int) kernel_stack + sizeof(kernel_stack);
    tss_init(kernel_esp);

    extern void tss_load(void);
    tss_load();

    if (mbinfo->mods_count != 1) {
        fb_write("ERROR: no module!", 17, FB_WHITE, FB_BLACK);
        while (1) {}
    }

    /* allocate page frames for user code and stack */
    unsigned int code_frame  = pmm_alloc();
    unsigned int stack_frame = pmm_alloc();
    unsigned int user_esp    = stack_frame + 4096;

    /* copy the user program into its code frame */
    memcpy_local((unsigned char *) code_frame,
                 (unsigned char *) prog_start,
                 prog_size);

    fb_write("Entering user mode...", 21, FB_WHITE, FB_BLACK);

    /* drop to PL3 — never returns */
    enter_usermode(code_frame, user_esp);

    while (1) {}
}
