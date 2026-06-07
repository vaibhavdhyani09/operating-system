#ifndef INCLUDE_FS_H
#define INCLUDE_FS_H

/* Maximum length of a file name, including the terminating NUL. */
#define FS_NAME_MAX 32

/* Magic number stored at the start of the file system image, used to
 * check that GRUB actually loaded our file system module and not
 * some random bytes. "OSFS" in little-endian. */
#define FS_MAGIC 0x53465330   /* '0','S','F','S' */

/* On-disk (in this case, in-RAM) header for the whole file system.
 * The builder tool writes this, the kernel reads it. */
struct fs_header {
    unsigned int magic;        /* must equal FS_MAGIC */
    unsigned int file_count;   /* number of fs_entry records that follow */
} __attribute__((packed));

/* On-disk metadata for one file. The file bodies are stored
 * contiguously after the table of fs_entry records. `offset` is
 * relative to the start of the file system image. */
struct fs_entry {
    char         name[FS_NAME_MAX];
    unsigned int offset;       /* byte offset of the body within the image */
    unsigned int size;         /* size of the body in bytes */
} __attribute__((packed));

/* Hand the file system the physical/linear address where GRUB loaded
 * the module. Returns 1 on success, 0 if the magic number is wrong. */
int fs_init(unsigned int image_addr);

/* Number of files in the mounted image (0 if not mounted). */
unsigned int fs_file_count(void);

/* Return the name of file number `index`, or 0 if out of range. */
const char *fs_name(unsigned int index);

/* Find a file by name. Returns its index, or -1 if not found. */
int fs_find(const char *name);

/* Read up to `len` bytes from file `index` starting at `offset` into
 * `buf`. Returns the number of bytes actually copied. */
unsigned int fs_read(unsigned int index, unsigned int offset,
                     char *buf, unsigned int len);

/* Size of file `index` in bytes (0 if out of range). */
unsigned int fs_size(unsigned int index);

#endif /* INCLUDE_FS_H */
