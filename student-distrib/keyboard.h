#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "i8259.h"

/*IRQ_NUMBER of keyboard in the IDT*/
#define KEYBOARD_IRQ_NUM 1

/*ports for the keyboard*/
#define KEYBOARD_PORT_DATA       0x60
#define KEYBOARD_PORT_CMD        0x64
#define MAX_BUFFER_SIZE 127
#define KEY_COUNT 58

#define KEYMODE 4

/*Keybaorad input data for special keys*/
#define CAPSLOCK_PRESSED      0x3A
#define CAPSLOCK_RELEASED    0xBA

#define LEFT_SHIFT_PRESSED    0x2A
#define RIGHT_SHIFT_PRESSED   0x36

#define LEFT_SHIFT_RELEASED  0xAA
#define RIGHT_SHIFT_RELEASED 0xB6

#define LEFT_ALT_PRESSED      0x38
#define LEFT_ALT_RELEASED    0xB8

#define LEFT_CTRL_PRESSED     0x1D
#define LEFT_CTRL_RELEASED   0x9D

#define TAB 0x0F

#define BAKCSPACE 0x0E


/*Functions define*/
void keyboard_init(void); 
void handle_keyboard_irq(void);

int handle_special_key(uint8_t keypressed) ;
void clean_buffer(void);
char keyboard_read(int index);
int check_enter(void);
#endif
