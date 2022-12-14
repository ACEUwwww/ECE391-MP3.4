#ifndef CURSOR_H
#define CURSOR_H

#include "types.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor(void);
void update_cursor(int x, int y);

#endif
