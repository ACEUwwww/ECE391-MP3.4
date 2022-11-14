#include "cursor.h"
#include "lib.h"
#define NUM_COLS    80
#define VIDEO       0xB8000

/* Referenced from https://wiki.osdev.org/Text_Mode_Cursor */

/* enable_cursor
    Description: enable the cursor
    Input: cursor_start, cursor_end
    Output: none
    Return value: none
    Other affect: none
*/
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x0A, 0x3D4);
	outb((inb(0x3D5) & 0xC0) | cursor_start, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

/* disable_cursor
    Description: disable the cursor
    Input: none
    Output: none
    Return value: none
    Other affect: none
*/
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

/* update_cursor
    Description: update cursor on the screen
    Input: x -- column
		   y -- row
    Output: none
    Return value: none
    Other affect: none
*/
void update_cursor(int x, int y)
{
	uint16_t pos = NUM_COLS * y + x;
 
	outb(0x0F, 0x3D4);
	outb((uint8_t) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((uint8_t) ((pos >> 8) & 0xFF), 0x3D5);
}





