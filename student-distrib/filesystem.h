/*Header file for file system driver. Created by YG*/
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
// Including all related headers
#include "types.h"

/* Define "magicnumbers"*/
// Used for files
#define     FILE_NAME_LENGTH 32
#define     DENTRY_LENGTH 63
#define     DENTRY_RESERVED 24          // reserved in dentry_t
#define     BOOTBLOCK_RESERVED 52       // reserved in bootblock
#define     MAX_FILE_NUMBER 63          // first also included
#define     MAX_FILE_SIZE 4190208     
#define     DATA_BLOCK_SIZE 4096
#define     INDEX_NODE_DB ((DATA_BLOCK_SIZE - 4)/4)
#define     FDARRAY_MAX 8
// Used for fileot
#define     FILEOT_ENTRIES 4
#define     rtc_in_fot 0
#define     dir_in_fot 1
#define     file_in_fot 2
#define     ter_in_fot 3


/* Define structures for file systems*/
// dentry
typedef struct dentry_t{
    int8_t file_name[FILE_NAME_LENGTH];
    int32_t file_type;             // 4B = 32 bits
    int32_t inode_index;           // 4B = 32 bits
    int8_t reserved[DENTRY_RESERVED]; // 24B = 8bits * 24 of reserved stuff
}dentry_t;
// bootblock
typedef struct bootblock{
    int32_t dentry_num;            // 4B
    int32_t inodes_num;            // 4B
    int32_t data_num;
    int8_t  reserved[BOOTBLOCK_RESERVED];
    dentry_t dentry[DENTRY_LENGTH];
}bootblock;
// one for each indexnode
typedef struct indexnode{
    uint32_t file_length;
    uint32_t data_blocks[INDEX_NODE_DB];
}indexnode;
//datablock should be uint8_t datablock[DATA_BLOCK_SIZE]. No need for a new struct.
typedef struct{
    uint8_t datablock[DATA_BLOCK_SIZE];
}datab;
/* Define functions required*/
int filesystem_init(bootblock *modstart);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


/* Open file. 1 on success, -1 on failure*/
int file_open(const char* fname);
/* Close file. Always return success*/
int file_close(const char* fname);
/* Read file. 1 on success, -1 on failure*/
int file_read(const char* fname, uint32_t offset, void* buf, int32_t length);
/* Write to file. Always failure*/
int file_write(int32_t fd, const void* buf, int32_t nbytes);
/* Open  directory. 1 on success, -1 on failure*/
int directory_open(const char* fname);
/* Close  directory. Always return success*/
int directory_close(const char* fname);
/* Read  directory. 1 on success, -1 on failure*/
int directory_read(const char* fname, uint32_t offset, void* buf, int32_t length);
/* Write to  directory. Always failure*/
int directory_write(int32_t fd, const void* buf, int32_t nbytes);

// test helpers
void list_all_file();
void read_test_help(int filechosen);


#endif

