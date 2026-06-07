#include "vfs.h"
#include "fs.h"

int vfs_mount(unsigned int image_addr)
{
    /* delegate to the one concrete file system we have */
    return fs_init(image_addr);
}

int vfs_open(const char *name, struct vfs_file *file)
{
    int index = fs_find(name);
    if (index < 0) {
        file->index    = -1;
        file->position = 0;
        return 0;
    }
    file->index    = index;
    file->position = 0;
    return 1;
}

unsigned int vfs_read(struct vfs_file *file, char *buf, unsigned int len)
{
    unsigned int n;

    if (file->index < 0) {
        return 0;   /* not an open file */
    }

    n = fs_read((unsigned int) file->index, file->position, buf, len);
    file->position += n;
    return n;
}
