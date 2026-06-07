/* mkfs.c - host build tool. NOT part of the kernel.
 * Builds a read-only file system image from a list of input files.
 *
 *   Usage:  ./mkfs out.img file1 file2 file3 ...
 *
 * Image layout (must match fs.h in the kernel):
 *   [ fs_header ][ fs_entry * N ][ body0 ][ body1 ] ... [ bodyN-1 ]
 *
 * Compile on the host with the system gcc (not the kernel CFLAGS):
 *   gcc -Wall -Wextra -o mkfs mkfs.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FS_NAME_MAX 32
#define FS_MAGIC    0x53465330u   /* keep in sync with fs.h */

struct fs_header {
    unsigned int magic;
    unsigned int file_count;
} __attribute__((packed));

struct fs_entry {
    char         name[FS_NAME_MAX];
    unsigned int offset;
    unsigned int size;
} __attribute__((packed));

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage: %s out.img file1 [file2 ...]\n", argv[0]);
        return 1;
    }

    int count = argc - 2;                 /* number of input files */
    struct fs_entry *table = calloc((size_t) count, sizeof(*table));
    if (!table) { perror("calloc"); return 1; }

    /* Body data starts right after the header and the entry table. */
    unsigned int header_size = sizeof(struct fs_header)
                             + (unsigned int) count * sizeof(struct fs_entry);

    /* First pass: read each file into memory and record its metadata. */
    char        **bodies = calloc((size_t) count, sizeof(char *));
    unsigned int  cursor = header_size;
    int           i;

    for (i = 0; i < count; i++) {
        const char *path = argv[i + 2];

        FILE *f = fopen(path, "rb");
        if (!f) { perror(path); return 1; }

        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (sz < 0) { perror("ftell"); return 1; }

        bodies[i] = malloc((size_t) sz);
        if (sz > 0 && fread(bodies[i], 1, (size_t) sz, f) != (size_t) sz) {
            perror("fread"); return 1;
        }
        fclose(f);

        /* store only the base name, capped to FS_NAME_MAX-1 chars */
        const char *base = strrchr(path, '/');
        base = base ? base + 1 : path;

        memset(table[i].name, 0, FS_NAME_MAX);
        strncpy(table[i].name, base, FS_NAME_MAX - 1);

        table[i].offset = cursor;
        table[i].size   = (unsigned int) sz;
        cursor += (unsigned int) sz;
    }

    /* Second pass: write header, table, then all bodies. */
    FILE *out = fopen(argv[1], "wb");
    if (!out) { perror(argv[1]); return 1; }

    struct fs_header hdr;
    hdr.magic      = FS_MAGIC;
    hdr.file_count = (unsigned int) count;

    fwrite(&hdr,  sizeof(hdr), 1, out);
    fwrite(table, sizeof(*table), (size_t) count, out);
    for (i = 0; i < count; i++) {
        fwrite(bodies[i], 1, table[i].size, out);
    }
    fclose(out);

    printf("wrote %s: %d file(s), %u bytes total\n",
           argv[1], count, cursor);
    return 0;
}
