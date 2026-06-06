#ifndef INCLUDE_PMM_H
#define INCLUDE_PMM_H

void pmm_init(unsigned int mem_upper, unsigned int kernel_phys_end);
unsigned int pmm_alloc(void);
void pmm_free(unsigned int addr);

#endif
