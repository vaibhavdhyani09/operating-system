#include "pmm.h"

/* each bit in the bitmap represents one 4KB page frame */
#define FRAME_SIZE      4096
#define MAX_FRAMES      32768       /* supports up to 128 MB of RAM */
#define BITMAP_SIZE     (MAX_FRAMES / 32)

static unsigned int bitmap[BITMAP_SIZE];  /* 0 = free, 1 = used */
static unsigned int total_frames = 0;

/* mark a frame as used */
static void set_frame(unsigned int frame)
{
    bitmap[frame / 32] |= (1 << (frame % 32));
}

/* mark a frame as free */
static void clear_frame(unsigned int frame)
{
    bitmap[frame / 32] &= ~(1 << (frame % 32));
}

/* check if a frame is free */
static unsigned int test_frame(unsigned int frame)
{
    return bitmap[frame / 32] & (1 << (frame % 32));
}

void pmm_init(unsigned int mem_upper, unsigned int kernel_phys_end)
{
    unsigned int i;

    /* mem_upper is KB above 1MB, convert to total bytes */
    unsigned int total_memory = (mem_upper * 1024) + 0x100000;
    total_frames = total_memory / FRAME_SIZE;

    /* start by marking everything as used */
    for (i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    /* free all frames above 1MB up to total memory */
    for (i = 0x100000 / FRAME_SIZE; i < total_frames; i++) {
        clear_frame(i);
    }

    /* re-mark frames used by the kernel as taken */
    unsigned int kernel_frames = (kernel_phys_end / FRAME_SIZE) + 1;
    for (i = 0; i < kernel_frames; i++) {
        set_frame(i);
    }

    /* mark first 1MB as used (BIOS, GRUB etc.) */
    for (i = 0; i < 0x100000 / FRAME_SIZE; i++) {
        set_frame(i);
    }
}

/* returns physical address of a free frame, or 0 if none */
unsigned int pmm_alloc(void)
{
    unsigned int i, j;
    for (i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0xFFFFFFFF) {       /* at least one bit free */
            for (j = 0; j < 32; j++) {
                if (!(bitmap[i] & (1 << j))) {
                    set_frame(i * 32 + j);
                    return (i * 32 + j) * FRAME_SIZE;
                }
            }
        }
    }
    return 0;   /* out of memory */
}

void pmm_free(unsigned int addr)
{
    clear_frame(addr / FRAME_SIZE);
}
