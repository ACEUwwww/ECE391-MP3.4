
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "terminal.h"

static char keyboard_buffer[MAX_BUFFER_SIZE+1];
uint8_t buffer_position = 0;
int32_t fd = 0;
/*States varibles*/
uint8_t Ctrl_pressed = 0 ;
uint8_t Shift_pressed= 0 ;
uint8_t Capslock_pressed = 0;
uint8_t Capslock_state = 0 ;
uint8_t Bakcspace_pressed = 0 ;
uint8_t Alt_pressed = 0 ;
const char* fname ;

/*8 stands for backspace*/
/* From  from http://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode */
static uint8_t scancode_map[KEY_COUNT][KEYMODE] ={
       {   0,0,0,0  } ,
       {   0,0,0,0  } ,
       { '1','!','1','!' } ,
       { '2','@','2','@' } ,
       { '3','#','3','#' } ,
       { '4','$','4','$' } ,
       { '5','%','5','%' } ,
       { '6','^','6','^' } ,
       { '7','&','7','&' } ,
       { '8','*','8','*' } ,
       { '9','(','9','(' } ,
       { '0',')','0',')' } ,
       { '-','_','-','_' } ,
       { '=','+','=','+' } ,
       {   8,8,8,8 } ,
       {   0,0,0,0 } ,
       { 'q','Q','Q','q' } ,
       { 'w','W','W','w' } ,
       { 'e','E','E','e' } ,
       { 'r','R','R','r' } ,
       { 't','T','T','t' } ,
       { 'y','Y','Y','y' } ,
       { 'u','U','U','u' } ,
       { 'i','I','I','i' } ,
       { 'o','O','O','o' } ,
       { 'p','P','P','p' } ,
       { '[','{','[','{' } ,
       { ']','}',']','}' } ,
       {  '\n','\n','\n','\n' } ,
       {   0,0,0,0   } ,
       { 'a','A','A','a' } ,
       { 's','S','S','s' } ,
       { 'd','D','D','d' } ,
       { 'f','F','F','f' } ,
       { 'g','G','G','g' } ,
       { 'h','H','H','h' } ,
       { 'j','J','J','j' } ,
       { 'k','K','K','k' } ,
       { 'l','L','L','l' } ,
       { ';',':',':',':' } ,
       {  39,34,39,34  } ,
       { '`','~','`','~'} ,
       {   0,0,0,0  } ,
       { '\\','|','\\','|'} ,
       { 'z','Z','Z','z' } ,
       { 'x','X','X','x' } ,
       { 'c','C','C','c' } ,
       { 'v','V','V','v' } ,
       { 'b','B','V','b'} ,
       { 'n','N','N','n' } ,
       { 'm','M','M','m' } ,
       { ',','<',',','<' } ,
       { '.','>','.','>' } ,
       { '/','?','/','?' } ,
       {   0,0,0,0   } ,
       {   0,0,0,0  } ,
       {   0,0,0,0  } ,
       { ' ',' ',' ',' ' } ,
}; 

/* 
 *  keyboard_init
 *  DESCRIPTION: initiaiize the keyboard by notice the PIC to accept interrupt from keyboard.
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: initiaiize the keyboard by notice the PIC to accept interrupt from keyboard.
 */

void keyboard_init()
{
    enable_irq(KEYBOARD_IRQ_NUM);
    return;
}

/* 
 *  keyboard_read
 *  DESCRIPTION: read the keyboard
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: none
 */
char keyboard_read(int index)
{
    return keyboard_buffer[index] ;
}

/* 
 *  handle_keyboard_irq
 *  DESCRIPTION: handle the keyboard irq, only singel press is considered
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: handle the keyboard irq, only singel press is considered
 */

void handle_keyboard_irq()
{
    /*before we start to handle the keyboard irq, we firstly send an eoi message to the PIC */
    send_eoi(KEYBOARD_IRQ_NUM);
    /*get the key pressed by the keyboard*/
    /*low 8 bits from Keyboard_port_data with 32bits input*/
    uint8_t scancode =  inb(KEYBOARD_PORT_DATA) & 0xFF; 
    char key_pressed;

    /*handle special key*/
    int special_key_handle;
    special_key_handle = handle_special_key(scancode);
    if (special_key_handle)
        return ;

    /*put the character on the screen*/
    if (scancode<58) /*58 stands for pressing max size */
    {
        /* Handle with shift and capslock */
        if (Shift_pressed && Capslock_state)
            key_pressed = scancode_map[scancode][3];

        else if (Shift_pressed && !Capslock_state)
            key_pressed = scancode_map[scancode][1];

        else if (!Shift_pressed && Capslock_state)
            key_pressed = scancode_map[scancode][2];

        else if (!Shift_pressed && !Capslock_state)
            key_pressed = scancode_map[scancode][0];

        /* Handle the Crtl-L part*/
        if (Ctrl_pressed && (key_pressed == 'L' || key_pressed == 'l'))
        {
            clean_screen();
            clean_buffer();
            return;
        }
        
        /* Handle enter */
        if (key_pressed == '\n')
        {
            keyboard_buffer[buffer_position] = '\n';
            putc(key_pressed);
            return;
        }

        /* Handle normal keyboard input */
        if ((buffer_position < MAX_BUFFER_SIZE)  && (key_pressed != '\n'))
        {
            keyboard_buffer[buffer_position] = key_pressed ;
            buffer_position ++ ;
            putc(key_pressed);
        }
    }

    return;

}


/* 
 *  handle_special_key
 *  DESCRIPTION: handle_special_key
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: none
 */
int handle_special_key(uint8_t scancode)
{
    int i;
    switch(scancode)
    {
        case CAPSLOCK_PRESSED:
            Capslock_pressed = 1;
            Capslock_state = (Capslock_state == 0) ? 1 : 0 ;
            return 1;
        case CAPSLOCK_RELEASED:
            Capslock_pressed = 0 ;
            return 1;
        case LEFT_SHIFT_PRESSED:
            Shift_pressed = 1;
            return 1;
        case RIGHT_SHIFT_PRESSED:
            Shift_pressed = 1;
            return 1;
        case LEFT_SHIFT_RELEASED:
            Shift_pressed = 0;
            return 1;
        case RIGHT_SHIFT_RELEASED:
            Shift_pressed = 0;
            return 1;
        case LEFT_ALT_PRESSED:
            Alt_pressed = 1;
            return 1;
        case LEFT_ALT_RELEASED:
            Alt_pressed = 0;
            return 1;
        case LEFT_CTRL_PRESSED:
            Ctrl_pressed = 1;
            return 1;
        case LEFT_CTRL_RELEASED:
            Ctrl_pressed = 0;
            return 1;
        /*handle backspace*/
        case BAKCSPACE:
            if (buffer_position > 0 )
            {
                buffer_position-- ;
                backspace_handler();
            }
            return 1;
        /*handle tab*/
        case TAB:
            for (i=0;i<4;i++)
            {
                if (buffer_position < MAX_BUFFER_SIZE)
                    {
                        keyboard_buffer[buffer_position]= ' ';
                        buffer_position ++ ;
                        putc(' ');
                    }
            }
            return 1;
        /*return 0 if no special key is handled*/
        default:
            return 0;
    }
}

/* 
 *  clean_buffer
 *  DESCRIPTION: clean the keyboard buffer
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: none
 */
void clean_buffer(void)
{
    int i;
    for (i=0;i<128;i++)
    {
        keyboard_buffer[i] = '\0';
    }
    buffer_position = 0;
    return;
}

int check_enter(void)
{
    if(keyboard_buffer[buffer_position] == '\n')
        return 1;
    else 
        return 0;
}
