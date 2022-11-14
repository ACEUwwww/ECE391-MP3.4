/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */


#include "i8259.h"
#include "lib.h"

/* Ports that each PIC sits on */
#define MASTER_8259_PORT    0x20
#define SLAVE_8259_PORT     0xA0

/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI                 0x60

/* Initialize the 8259 PIC */
void i8259_init(void) {
    outb(0xFF,MASTER_8259_PORT+1);
    outb(0xFF,SLAVE_8259_PORT+1);

    outb(ICW1,MASTER_8259_PORT);                  // select 8259A-1 init
    outb(ICW2_MASTER, MASTER_8259_PORT+1);        // mapped to 0x20-0x27
    outb(ICW3_MASTER,MASTER_8259_PORT+1);         // 8259A-1 is a primary on master's irq2
    outb(ICW4, MASTER_8259_PORT+1);               

    outb(ICW1, SLAVE_8259_PORT);                  // select 8259A-2 init
    outb(ICW2_SLAVE, SLAVE_8259_PORT+1);          // mapped to 0x28-0x2f
    outb(ICW3_SLAVE, SLAVE_8259_PORT+1);          // 8259A-2 is a secondary on master's irq2
    outb(ICW4, SLAVE_8259_PORT+1);            

    outb(master_mask, MASTER_8259_PORT+1);
    outb(slave_mask, SLAVE_8259_PORT+1);

    master_mask = 0xFF;
    slave_mask = 0xFF;
    enable_irq(2);
    enable_irq(8);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    // unsigned long flags;
    int signal;
    // cli_and_save(flags);                            // Below is critical section
    if(irq_num<0 || irq_num>15){return;}            // If invalid, return
    if(irq_num <= 7)                                // 0-7 is master
    {   
        signal = 1 << irq_num;                      
        master_mask = master_mask & (~signal);      // update master_mask
        outb(master_mask, MASTER_8259_PORT+1);
        if(irq_num == ICW3_SLAVE)                   // if connected to the slave, then enable all on slave
        {
            slave_mask = 0;                         // mask none
            outb(slave_mask, SLAVE_8259_PORT + 1);
        }
    }
    else
    {   
        signal = 1 << (irq_num - 8);
        slave_mask = slave_mask & (~signal);        // update slave mask
        outb(slave_mask, SLAVE_8259_PORT+1);
        signal = 1 << 2;                      
        master_mask = master_mask & (~signal);      // update master_mask
        outb(master_mask, MASTER_8259_PORT+1);
    }
    // restore_flags(flags);
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    // unsigned long flags;
    int signal;
    // cli_and_save(flags);                             // Below is critical section
    if(irq_num<0 || irq_num>15){return;}                // If invalid, return
    if(irq_num <= 7)
    {   
        signal = 1 << irq_num;
        master_mask = master_mask | signal;             // update master_mask
        outb(master_mask, MASTER_8259_PORT + 1);
        if(irq_num == ICW3_SLAVE)                       // if connected to the slave, then enable all on slave
        {   
            slave_mask = 0xFF;                          // mask all
            outb(slave_mask, SLAVE_8259_PORT + 1);
        }
    }
    else
    {   
        signal = 1 << (irq_num - 8);
        slave_mask = slave_mask | signal;               // update slave mask
        outb(slave_mask, SLAVE_8259_PORT);
    }
    // restore_flags(flags);
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    // unsigned long flags;
    // cli_and_save(flags);
    int signal;                                         
    if(irq_num<0 || irq_num>15){return;}                // return if invalid
    if(irq_num <= 7 && irq_num != 2)                    // if refer to the master port
    {
        signal = irq_num | EOI;
        outb(signal, MASTER_8259_PORT);                 // send to the master port
    }
    else                                                
    {
        signal = (irq_num-8) | EOI;
        outb(signal, SLAVE_8259_PORT);                  // shift and send to the slave port
        outb((2|EOI), MASTER_8259_PORT);                // send to pin2 to the master port
    }
    // restore_flags(flags);
}
