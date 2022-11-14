#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

int32_t terminal_open(const char* filename);
int32_t terminal_close(const char* fname);
int32_t terminal_write(int32_t fd, const void* input_buf, int32_t nbytes);
int32_t terminal_read(const char* fname,uint32_t fd, void* buf, int32_t nbytes);
void clean_terminal_buffer();



#endif
