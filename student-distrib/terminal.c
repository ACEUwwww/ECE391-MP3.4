#include "terminal.h"
#include "keyboard.h"
#include "lib.h"

char terminal_buffer[MAX_BUFFER_SIZE+1];
/* 
 * terminal_open
 *   DESCRIPTION: initializing the terminal for further use.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */

int32_t terminal_open(const char* filename)
{
    return 1 ;
}

/* 
 * terminal_close
 *   DESCRIPTION: cleaning terminal for further use.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */

int32_t terminal_close(const char* fname)
{
    clean_buffer();
    clean_terminal_buffer();
    return 1;
}

/* 
 * terminal_read
 *   DESCRIPTION: read from keyboard buffer to terminal buffer
 *   INPUTS: const char* fname,uint32_t fd, void* buf, int32_t nbytes)
 *   OUTPUTS: none
 *   RETURN VALUE: i - the length read
 *   SIDE EFFECTS: none
 */
int32_t terminal_read(const char* fname,uint32_t fd, void* buf, int32_t nbytes)
{
    char* t_buf = buf ;
    if (t_buf==NULL){return -1;}
    if (nbytes < 0){return -1;}
    // we have 1024 as the max buf size
    if (nbytes >= 1024){
        return -1;
    }   
    int i=0;
    while((check_enter() == 0)){}
    while(keyboard_read(i) != '\n'){
        t_buf[i] = keyboard_read(i);
        i++;
    }
    clean_buffer();
    return i;
}

/* 
 * terminal_write
 *   DESCRIPTION: write from terminal to screen and clean terminal buffer.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: i - write length
 *   SIDE EFFECTS: none
 */

int32_t terminal_write(int32_t fd, const void* input_buf, int32_t nbytes)
{
    const char* buf = input_buf;
    if(buf == NULL){
        return -1;
    }
    int32_t count;
    for(count = 0; count < nbytes; count++){
        putc(buf[count]);
    }
    return count;
}

/* 
 * terminal_write
 *   DESCRIPTION: clean the terminal buffer.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void clean_terminal_buffer()
{
    int i;
    /*Clean the keyboard buffer*/
    for (i=0;i<MAX_BUFFER_SIZE+1;i++)
    {
        terminal_buffer[i] = '\0' ;
    }
    return ;
}
