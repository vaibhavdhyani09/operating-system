#include "multiboot.h"
#include "fb.h"
#include "vfs.h"

void kmain(unsigned int ebx)
{
    fb_clear();

    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    unsigned int *mods_addr;
    unsigned int  image_addr;
    struct vfs_file file;
    char buf[256];
    unsigned int n;

    /* confirm GRUB loaded exactly our one module (the FS image) */
    if (mbinfo->mods_count != 1) {
        fb_write_str("ERROR: fs module not loaded!", FB_WHITE, FB_BLACK);
        return;
    }

    /* the module's start address is the first word at mods_addr */
    mods_addr  = (unsigned int *) mbinfo->mods_addr;
    image_addr = mods_addr[0];

    /* mount the read-only file system that lives in that module */
    if (!vfs_mount(image_addr)) {
        fb_write_str("ERROR: bad fs magic - not mounted!", FB_WHITE, FB_BLACK);
        return;
    }

    fb_write_str("File system mounted.\n", FB_GREEN, FB_BLACK);

    /* open a file by name and print its contents */
    if (vfs_open("hello.txt", &file)) {
        fb_write_str("Contents of hello.txt:\n", FB_WHITE, FB_BLACK);
        while ((n = vfs_read(&file, buf, sizeof(buf) - 1)) > 0) {
            buf[n] = '\0';
            fb_write_str(buf, FB_WHITE, FB_BLACK);
        }
    } else {
        fb_write_str("hello.txt not found.\n", FB_WHITE, FB_BLACK);
    }
}
