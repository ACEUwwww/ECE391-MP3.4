/* paging.c - Functions to initializing page
 * 
 */

#include "paging.h"
#include "lib.h"
#include "x86_desc.h"

/* 
 * paging_init
 *   DESCRIPTION: initializing the page, initialize all PTE and PDE in page directory
                  and page tables
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */

void paging_init()
{
    int i,j; // set varibles for loop
    for(i=0;i<DIC_SIZE;i++)
    { 
        if(i==0) // for 0-4MB VM, use 4KB page
        {
            for(j=0;j<TABLE_SIZE;j++)//for each 4KB page
            {
                PT_init(j);
            }
            DT_init(i);  
        }
        else // for 4MB-4GB, use 4MB page
        {
            DT_init(i); 
        }
    }
    set_crreg(); // set CR0 CR3 CR4
    return;
}

/* 
 * PT_init
 *   DESCRIPTION: initializing the page table, initialize all PTE in 4kb page table 
                  and check video memory
 *   INPUTS: index -- from 0 - 1023, each PTE's offset
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void PT_init(int index)
{
    PT[index] = 0x00000000; // set all bits to zero for convenience
    if((index<<12) != VIDEO_MEM) // check the page is not for video memory
    {
        PT[index] = PT[index] | PTE_RW;
        PT[index] = PT[index] | ((index)<<12); // 0-11bit for offset
    }
    else // page for video memory
    {
        PT[index] = PT[index] | PTE_P; //set present to 1
        PT[index] = PT[index] | PTE_RW;
        PT[index] = PT[index] | PTE_G;
        PT[index] = PT[index] | ((index)<<12); // 0-11bit for offset
    }
    return;
}

/* 
 * DT_init
 *   DESCRIPTION: initializing the page directory, initialize all PDE in 4GB VM 
                  and check 0-4MB, kernel and other VM not used now
 *   INPUTS: index -- from 0 - 1023, each PDE's offset
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void DT_init(int index)
{
    DT[index] = 0x00000000; // set all bits to zero for convenience
    if(index == 0) // for 0-4MB
    {
        DT[index] = DT[index] | PDE_P; // set present bit
        DT[index] = DT[index] | PDE_RW;
        DT[index] = DT[index] | PDE_US;
        DT[index] = DT[index] | (((uint32_t)PT >> 12) << 12 ); // store addr of PT in DT[0], 0-11bit for offset
    }
    else if(index == 1)// for kernel, 4-8MB
    {
        DT[index] = DT[index] | PDE_P; // set present bit
        DT[index] = DT[index] | PDE_RW;
        DT[index] = DT[index] | PDE_PS; // set page size
        DT[index] = DT[index] | PDE_G;
        DT[index] = DT[index] | (1<<22); // store 4MB page, 0-21bit for offset
    }
    else{
        DT[index] = DT[index] | PDE_PS; // set page size
        DT[index] = DT[index] | PDE_G;
        DT[index] = DT[index] | (1<<22); // store 4MB page, 0-21bit for offset
    }
}

/* 
 * set_crreg
 *   DESCRIPTION: load physical address of DT to CR3, set bit4 of CR4 to 1 for mixed size
                  and set bit31 of CR0 to 1 for enable 
 *   INPUTS:  none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void set_crreg()
{
    asm volatile(
    //load physical address of DT to CR3
    "movl $DT, %%eax;"
    "andl $0xFFFFFC00, %%eax;"
    "movl %%eax, %%cr3;"

    // set bit4 of CR4 to 1 for mixed size
    "movl %%cr4, %%eax;"
    "orl $0x00000010, %%eax;"
    "movl %%eax, %%cr4; "  
    //set bit31 of CR0 to 1 for enable 
    "movl %%cr0, %%eax;"
    "orl $0x80000000, %%eax;"
    "movl %%eax, %%cr0;"  
    :
    :
    : "eax");
}

void tlb_flash()
{
    asm volatile(
        "movl %%cr3,%%eax     ;"
        "movl %%eax,%%cr3     ;"

        : : : "eax", "cc" 
    );
}





