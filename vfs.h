#ifndef INCLUDE_VFS_H
#define INCLUDE_VFS_H

/* An open "file handle". For now it just remembers which file in the
 * underlying read-only FS we're talking to and how far we've read. */
struct vfs_file {
    int          index;        /* index into the backing fs, -1 = closed */
    unsigned int position;     /* current read offset                    */
};

/* Mount the backing file system that lives at image_addr.
 * Returns 1 on success, 0 on failure. */
int vfs_mount(unsigned int image_addr);

/* Open a file by name. Fills in `file` and returns 1 on success,
 * 0 if the file does not exist. */
int vfs_open(const char *name, struct vfs_file *file);

/* Read up to len bytes from an open file, advancing its position.
 * Returns bytes read (0 at end of file or on a closed handle). */
unsigned int vfs_read(struct vfs_file *file, char *buf, unsigned int len);

#endif /* INCLUDE_VFS_H */
