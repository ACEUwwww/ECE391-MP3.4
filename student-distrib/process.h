#ifndef _PROCESS_H
#define _PROCESS_H

#include "types.h"
#include "x86_desc.h"
#include "filesystem.h"
#include "paging.h"
#include "terminal.h"
#include "fileoptable.h"

// variables we need
#define MAX_PROCESS_NUMBER 3
#define USER_MEM 0x08000000
#define USER_END 0x08400000
#define USER_PADDR 0x08048000
#define KERNEL_STACK_SIZE 0x2000
#define END_OF_KERNEL 0x00800000
#define USER_STACK_SIZE 0x00400000
#define EXE_CHECK1 0x7F
#define EXE_CHECK2 0x45
#define EXE_CHECK3 0x4C
#define EXE_CHECK4 0x46
#define PD_MASK 0x87
#define EXCEPTION_STATUS 0x0F
#define FDARRAY_MAX 8
#define STACK_SIZE 4
// file descriptor array
typedef struct fdarray{
    fileot *fileot_pointer;
    int32_t inode;
    uint32_t position;
    uint32_t flags;
} fdarray;

typedef struct PCB_t{
    fdarray fd_array[8];
    uint32_t pid;
    uint32_t parent_pid;
    uint32_t parent_ebp;
    uint32_t tss_esp0;
    uint32_t scheduled_esp;
    uint32_t scheduled_ebp;
    int8_t arg[FILE_NAME_LENGTH];
    // uint32_t scheduled_esp0;        
    // uint32_t current_fd;            
    // uint32_t mapped_video_addr;     
} PCB_t;


int32_t execute(const uint8_t* command);
int32_t halt(uint8_t status);
int32_t open (const uint8_t* filename);
int32_t close (int i);
int32_t sigreturn(void);
int32_t set_handler(int32_t signum, void* address);
int32_t getargs(uint8_t* buf, int32_t n);
int32_t vidmap(uint8_t** start);
int8_t initialize_PCB(int8_t* arg_command);
int32_t write(uint32_t offset, void* buf, int32_t n);
int32_t read(uint32_t offset, void* buf, int32_t n);
fdarray *getfdarray();
/* Initialize fdarray*/
void fdarray_init();

#endif 

