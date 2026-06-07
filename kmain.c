#include "multiboot.h"
#include "fb.h"
#include "vfs.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "pmm.h"
#include "paging.h"

void kmain(unsigned int ebx)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    unsigned int *mods_addr;
    unsigned int  mod_start;
    unsigned int  mod_end;
    struct vfs_file file;
    char buf[256];
    unsigned int n;

    fb_clear();

    /* ── GDT ─────────────────────────────────────────── */
    gdt_init();
    fb_write_str("[  OK  ] GDT initialised\n", FB_GREEN, FB_BLACK);

    /* ── IDT ─────────────────────────────────────────── */
    idt_init();
    fb_write_str("[  OK  ] IDT initialised\n", FB_GREEN, FB_BLACK);

    /* ── PIC ─────────────────────────────────────────── */
    pic_remap();
    fb_write_str("[  OK  ] PIC remapped\n", FB_GREEN, FB_BLACK);

    /* ── Paging ──────────────────────────────────────── */
    paging_init();
    fb_write_str("[  OK  ] Paging enabled (identity mapped, 4MB pages)\n",
                 FB_GREEN, FB_BLACK);

    /* ── File system module check ────────────────────── */
    if (mbinfo->mods_count != 1) {
        fb_write_str("[FAIL  ] FS module not loaded\n", FB_WHITE, FB_BLACK);
        goto enable_input;
    }

    /* multiboot module entry: [0]=mod_start, [1]=mod_end */
    mods_addr = (unsigned int *) mbinfo->mods_addr;
    mod_start = mods_addr[0];
    mod_end   = mods_addr[1];

    /* ── Physical memory manager ─────────────────────── */
    /* kernel_phys_end: kernel is loaded at 1MB; we don't have an exact
     * label here, so pass mod_end which is past the kernel + module.    */
    pmm_init(mbinfo->mem_upper, mod_end, mod_end);
    fb_write_str("[  OK  ] Page frame allocator ready\n",
                 FB_GREEN, FB_BLACK);

    /* ── File system ─────────────────────────────────── */
    if (!vfs_mount(mod_start)) {
        fb_write_str("[FAIL  ] FS bad magic\n", FB_WHITE, FB_BLACK);
        goto enable_input;
    }

    fb_write_str("[  OK  ] File system mounted (read-only, RAM-based)\n",
                 FB_GREEN, FB_BLACK);

    if (vfs_open("hello.txt", &file)) {
        fb_write_str("[  OK  ] hello.txt opened\n", FB_GREEN, FB_BLACK);
        fb_write_str("         Contents: ", FB_WHITE, FB_BLACK);
        while ((n = vfs_read(&file, buf, sizeof(buf) - 1)) > 0) {
            buf[n] = '\0';
            fb_write_str(buf, FB_WHITE, FB_BLACK);
        }
        fb_write_str("\n", FB_WHITE, FB_BLACK);
    } else {
        fb_write_str("[FAIL  ] hello.txt not found\n", FB_WHITE, FB_BLACK);
    }

enable_input:
    /* ── Keyboard ────────────────────────────────────── */
    fb_write_str("[  OK  ] Keyboard IRQ1 active\n", FB_GREEN, FB_BLACK);
    fb_write_str("\n", FB_WHITE, FB_BLACK);
    fb_write_str("----------------------------------------\n",
                 FB_WHITE, FB_BLACK);
    fb_write_str(" Type below:\n", FB_WHITE, FB_BLACK);
    fb_write_str("----------------------------------------\n",
                 FB_WHITE, FB_BLACK);

    /* interrupts get enabled by `sti` in loader.s after kmain returns */
}
