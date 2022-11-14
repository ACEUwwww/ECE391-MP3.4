#ifndef KEYBOARD_INTERRUPT_WRAPPER_H_
#define KEYBOARD_INTERRUPT_WRAPPER_H_

#ifndef ASM
    extern void keyboard_irq_wrap();
    extern void rtc_irq_wrap();

#endif

#endif
