#include "fs.h"

/* Pointers into the image GRUB loaded for us. The image is laid out as:
 *
 *   [ fs_header ][ fs_entry 0 ][ fs_entry 1 ] ... [ bodies ... ]
 *
 * Everything lives in RAM, loaded by GRUB before the kernel ran, so
 * this is a purely read-only file system (the chapter's first step). */

static unsigned char  *fs_base    = 0;   /* start of the whole image     */
static struct fs_header *fs_hdr   = 0;   /* parsed header                */
static struct fs_entry  *fs_table = 0;   /* array of file_count entries  */

int fs_init(unsigned int image_addr)
{
    fs_base = (unsigned char *) image_addr;
    fs_hdr  = (struct fs_header *) fs_base;

    if (fs_hdr->magic != FS_MAGIC) {
        /* not our file system - refuse to mount */
        fs_base  = 0;
        fs_hdr   = 0;
        fs_table = 0;
        return 0;
    }

    /* the entry table immediately follows the header */
    fs_table = (struct fs_entry *) (fs_base + sizeof(struct fs_header));
    return 1;
}

unsigned int fs_file_count(void)
{
    if (fs_hdr == 0) {
        return 0;
    }
    return fs_hdr->file_count;
}

const char *fs_name(unsigned int index)
{
    if (fs_hdr == 0 || index >= fs_hdr->file_count) {
        return 0;
    }
    return fs_table[index].name;
}

/* tiny local string compare so we don't need a libc */
static int fs_streq(const char *a, const char *b)
{
    unsigned int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return a[i] == b[i];   /* both ended at the same place */
}

int fs_find(const char *name)
{
    unsigned int i;
    if (fs_hdr == 0) {
        return -1;
    }
    for (i = 0; i < fs_hdr->file_count; i++) {
        if (fs_streq(fs_table[i].name, name)) {
            return (int) i;
        }
    }
    return -1;
}

unsigned int fs_size(unsigned int index)
{
    if (fs_hdr == 0 || index >= fs_hdr->file_count) {
        return 0;
    }
    return fs_table[index].size;
}

unsigned int fs_read(unsigned int index, unsigned int offset,
                     char *buf, unsigned int len)
{
    unsigned char *body;
    unsigned int   size;
    unsigned int   i;

    if (fs_hdr == 0 || index >= fs_hdr->file_count) {
        return 0;
    }

    size = fs_table[index].size;
    if (offset >= size) {
        return 0;   /* nothing to read past the end */
    }

    /* clamp the read so we never run off the end of the file */
    if (offset + len > size) {
        len = size - offset;
    }

    body = fs_base + fs_table[index].offset;
    for (i = 0; i < len; i++) {
        buf[i] = (char) body[offset + i];
    }
    return len;
}
