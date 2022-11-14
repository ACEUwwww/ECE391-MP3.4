/*File operation table. Created by Group44YG*/
// file operation table
#ifndef FILEOPTABLE_H
#define FILEOPTABLE_H


#include "types.h"
#define num_entries_for_fot 4
typedef struct fileot{
    int32_t (*open)     (const char* filename);
    int32_t (*close)    (const char* fname);
    int32_t (*read)     (const char* fname, uint32_t offset, void* buf, int32_t nbytes);
    int32_t (*write)    (int32_t fd, const void* buf, int32_t nbytes);
} fileot;
// file descriptor array

// create file_operation_table
fileot file_operation_table[num_entries_for_fot];


/* Initialize file operation table*/
void init_fot();
/* Initialize fdarray*/
void fdarray_init();

fileot get_terminal_fot(void);
fileot get_file_fot(void);
fileot get_dir_fot(void);
fileot get_rtc_fot(void);
/* Open file. 1 on success, -1 on failure*/
int rtcopen(const char* fname);
/* Close file. Always return success*/
int rtcclose(const char* fname);
/* Read file. 1 on success, -1 on failure*/
int rtcread(const char* fname, uint32_t offset, void* buf, int32_t length);
/* Write to file. Always failure*/
int rtcwrite(int32_t fd, const void* buf, int32_t nbytes);
/* Open  directory. 1 on success, -1 on failure*/


#endif
