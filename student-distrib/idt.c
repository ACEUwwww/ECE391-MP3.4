/* Created by Group44 YG*/
/* This file, together with idt.h, is used to initialize IDT.
    Not done yet.
    Should include more handlers and set up when more devices and exceptions comes.
*/
#include "idt.h"
#include "Keyboard_Interrupt_wrapper.h"
#include "syscall.h"

/* exception_handler
    Description: print out the error
    Input: index
    Output: print out the index and description
    Return value: nan
    Other affect: print out some value
*/

// handle exceptions (printing the error type)
void e0();
void e1();
void e2();
void e3();
void e4();
void e5();
void e6();
void e7();
void e8();
void e9();
void e10();
void e11();
void e12();
void e13();
void e14();
void e15();
void e16();
void e17();
void e18();
void e19();



#define num_exception 20        // total number of exceptions
#define itr_rtc 0x28            // place the rtc
#define itr_keyboard 0x21       // place the keyboard
#define idt_size 256            // 0-255
#define system_call  0x80

char* idt_name_table[num_exception] = {
    "Divide Error",
    "RESERVED",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "e10 Reserved",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "e15 Reserved",
    "x87 FPU Floating-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception"
};
void e0()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[0]);
    while(1);
}
void e1()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[1]);
    while(1);
}
void e2()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[2]);
    while(1);
}
void e3()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[3]);
    while(1);
}
void e4()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[4]);
    while(1);
}
void e5()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[5]);
    while(1);
}
void e6()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[6]);
    while(1);
}
void e7()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[7]);
    while(1);
}
void e8()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[8]);
    while(1);
}
void e9()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[9]);
    while(1);
}
void e10()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[10]);
    while(1);
}
void e11()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[11]);
    while(1);
}
void e12()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[12]);
    while(1);
}
void e13()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[13]);
    while(1);
}
void e14()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[14]);
    while(1);
}
void e15()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[15]);
    while(1);
}
void e16()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[16]);
    while(1);
}
void e17()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[17]);
    while(1);
}
void e18()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[18]);
    while(1);
}
void e19()
{
    printf("Error\n");
    printf("Error Description: %s\n", idt_name_table[19]);
    while(1);
}
/* idt_init
    Description: initialize idt
    Input: nan
    Output: nan
    Return value: nan
    Other affect: idt initialized
*/
void idt_init()
{   
    int init_loop;                  // used to loop to initalize all

    // some for loop
    for(init_loop = 0; init_loop < idt_size; init_loop++)
    {
        idt[init_loop].seg_selector = KERNEL_CS;
        idt[init_loop].reserved3 = 1;
        idt[init_loop].reserved2 = 1;
        idt[init_loop].reserved1 = 1;
        idt[init_loop].size = 1;
        idt[init_loop].reserved0 = 0;
        idt[init_loop].dpl = 0;
        idt[init_loop].present = 0;
    }
    SET_IDT_ENTRY(idt[0], e0);
    SET_IDT_ENTRY(idt[1], e1);
    SET_IDT_ENTRY(idt[2], e2);
    SET_IDT_ENTRY(idt[3], e3);
    SET_IDT_ENTRY(idt[4], e4);
    SET_IDT_ENTRY(idt[5], e5);
    SET_IDT_ENTRY(idt[6], e6);
    SET_IDT_ENTRY(idt[7], e7);
    SET_IDT_ENTRY(idt[8], e8);
    SET_IDT_ENTRY(idt[9], e9);
    SET_IDT_ENTRY(idt[10], e10);
    SET_IDT_ENTRY(idt[11], e11);
    SET_IDT_ENTRY(idt[12], e12);
    SET_IDT_ENTRY(idt[13], e13);
    SET_IDT_ENTRY(idt[14], e14);
    SET_IDT_ENTRY(idt[15], e15);
    SET_IDT_ENTRY(idt[16], e16);
    SET_IDT_ENTRY(idt[17], e17);
    SET_IDT_ENTRY(idt[18], e18);
    SET_IDT_ENTRY(idt[19], e19);
    for(init_loop = 0; init_loop < num_exception; init_loop++)
    {
        idt[init_loop].present = 1;
    }
    // set idt entry for interrupts
    SET_IDT_ENTRY(idt[itr_rtc], rtc_irq_wrap);
    idt[itr_rtc].reserved3 = 0;
    idt[itr_rtc].present = 1; 
    SET_IDT_ENTRY(idt[itr_keyboard], keyboard_irq_wrap); 
    idt[itr_keyboard].reserved3 = 0; 
    idt[itr_keyboard].present = 1; 
    SET_IDT_ENTRY(idt[system_call],systemcall);
    idt[system_call].dpl = 3;
    idt[system_call].present = 1;
}

