#ifndef _PAGING_H
#define _PAGING_H
#include "types.h"
#include "x86_desc.h"
// variables we need
#define TABLE_SIZE  1024
#define DIC_SIZE  1024
#define PTE_P   0x00000001
#define PTE_RW  0x00000002
#define PTE_US  0x00000004
#define PTE_G   0x00000100
#define PTE_ADDR   0x11111000
#define PDE_P   0x00000001
#define PDE_RW  0x00000002
#define PDE_US  0x00000004
#define PDE_PS  0x00000080
#define PDE_G   0x00000100
#define VIDEO_MEM   0x000B8000
#define VIDEO_END   0x000B8FFF
#define KERNEL_OFFSET 0x00400000
#define KERNEL_END 0x007FFFFF
#define VIDEO_INDEX 34
#define USER_VIDEO_MEM 0x08800000
// structures we need
uint32_t DT[DIC_SIZE]__attribute__((aligned(4096)));
uint32_t PT[TABLE_SIZE]__attribute__((aligned(4096)));
uint32_t PT_VIDMEM[TABLE_SIZE]__attribute__((aligned(4096)));

// functions we need
extern void paging_init();
extern void PT_init(int index);
extern void DT_init(int index);
void set_crreg();
void tlb_flash();

#endif
