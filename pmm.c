#include "pmm.h"

#define FRAME_SIZE   4096
#define MAX_FRAMES   32768
#define BITMAP_SIZE  (MAX_FRAMES / 32)

static unsigned int bitmap[BITMAP_SIZE];
static unsigned int total_frames = 0;

static void set_frame(unsigned int frame)
{
    bitmap[frame / 32] |= (1u << (frame % 32));
}

static void clear_frame(unsigned int frame)
{
    bitmap[frame / 32] &= ~(1u << (frame % 32));
}

void pmm_init(unsigned int mem_upper,
              unsigned int kernel_phys_end,
              unsigned int module_phys_end)
{
    unsigned int i;
    unsigned int total_memory = (mem_upper * 1024) + 0x100000;
    total_frames = total_memory / FRAME_SIZE;

    /* mark everything used */
    for (i = 0; i < BITMAP_SIZE; i++)
        bitmap[i] = 0xFFFFFFFF;

    /* free everything above 1MB */
    for (i = 0x100000 / FRAME_SIZE; i < total_frames; i++)
        clear_frame(i);

    /* re-mark first 1MB (BIOS/GRUB) */
    for (i = 0; i < 0x100000 / FRAME_SIZE; i++)
        set_frame(i);

    /* re-mark kernel frames */
    unsigned int kernel_frames = (kernel_phys_end / FRAME_SIZE) + 1;
    for (i = 0; i < kernel_frames; i++)
        set_frame(i);

    /* re-mark module frames so PMM never hands them out */
    unsigned int module_frames = (module_phys_end / FRAME_SIZE) + 1;
    for (i = 0; i < module_frames; i++)
        set_frame(i);
}

unsigned int pmm_alloc(void)
{
    unsigned int i, j;
    for (i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0xFFFFFFFF) {
            for (j = 0; j < 32; j++) {
                if (!(bitmap[i] & (1u << j))) {
                    set_frame(i * 32 + j);
                    return (i * 32 + j) * FRAME_SIZE;
                }
            }
        }
    }
    return 0;
}

void pmm_free(unsigned int addr)
{
    clear_frame(addr / FRAME_SIZE);
}
