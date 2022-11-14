#define ASM     1
#include "Keyboard_Interrupt_wrapper.h"

.global keyboard_irq_wrap, rtc_irq_wrap

/* keyboard_irq_wrap
 * DESCIRPTION: make all register saved before calling the function handle_keyboard_irq
 * INPUT:  none
 * OUTPUT: none
 * RETURN: none
 */
keyboard_irq_wrap:
    pushal                          # store all the registers
    pushfl
    call handle_keyboard_irq       # call the function                
    popfl      
    popal                           # restore the regesters
    iret

rtc_irq_wrap:
    pushal                          # store all the registers
    pushfl
    call     rtc_interrupt_handler   # call the function                
    popfl      
    popal                           # restore the regesters
    iret

