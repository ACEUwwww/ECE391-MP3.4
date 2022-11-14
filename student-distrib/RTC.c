
#include "i8259.h"
#include "lib.h"
#include "RTC.h"

/*
 * S
 *   DESCRIPTION: initialize and  avoid getting an undefined state.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
// some information from https://blog.csdn.net/weixin_29955689/article/details/116850755
// Refrence from: https://wiki.osdev.org/RTC
// need an entry to IDT here
void rtc_init(){
    // mask interrupt, such as irq_disable() 
    char prev;
    // irq_disable(RTCindex); set rtc with slave_mask
    /*maskcode = (1 << (RTC_IRQ_index-8));
    slave_mask |= maskcode; // mask the salve bit
    outb(slave_mask, SLAVE_8259_PORT);*/
    disable_irq(RTC_IRQ_index);

    // use Register B , at the same time, disable NMI
    outb(RegB, RTC_CMD); 
    // read Register B
    prev = inb(RTC_CMOS);
    // set the index again (a read will reset the index to register D)
    outb(RegB,RTC_CMD);		              
    // write the previous value ORed with 0x40. This turns on bit 6 of register B 
    // set PIE to 1, turning on IRQ 8
    outb(prev|bit6, RTC_CMOS);	  

    // set the frequency of 2hz (default), using A to handle the frequency
    // use Register B , at the same time, disable NMI
    outb(RegA, RTC_CMD);
    // read Register A
    prev = inb(RTC_CMOS);
    // set the index again
    outb(RegA,RTC_CMD);		              
    // write the previous value ORed with 0x40. This turns on bit 6 of register B 
    outb((prev & bits4to7) | getRate(1024), RTC_CMOS);
    
    // enable irq from RTC
    /*sti(); // include lib.h
    maskcode = ~(1 << (RTC_IRQ_index-8));
    slave_mask &= maskcode;
    outb(slave_mask, SLAVE_8259_PORT);*/
    enable_irq(RTC_IRQ_index);
}


int32_t getRate(int32_t freq)
{ 
    // can't include math.h here
    if(freq < 2 || freq > 1024){
        return -1;
    }
    int32_t ret = 0x0F;
    // double temp;
    // temp = log2(freq) - 1;
    // ret = (int) (0x0f - temp);
    // return ret;*/
    while( freq != 2 ){
        freq /= 2;
        ret -- ;
    }
    return ret;
}

volatile int rtc_interrupt = 0; // according to the max term numbers

/*
 * rtc_interrupt_handler
 *   DESCRIPTION: an interrupt handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Handle rtc interrupt, unmasking handled interrupt			   
 */
void rtc_interrupt_handler(){
	outb(RegC, RTC_CMD); 	//select register C and disable NMI
	inb(RTC_CMOS);			//the read value is useless
    rtc_interrupt = 1;
    //change interrupt status
	//test_interrupts();
	//send eoi the RTC line, mask it
	send_eoi(RTC_IRQ_index);
	
    return;
}

/*
 * rtc_read
 *   DESCRIPTION: return when 1024/freq times interrupt happened
 *   INPUTS: freq
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: never stop if the interript is not enough
 */
int rtc_read(int freq)
{
    int i;
    for (i = 0; i < (16/issue.rtc_freq); ++i){
        while( rtc_interrupt == 0 ){} ; 
        // wait until rtc interrupt happens
        cli();
        rtc_interrupt = 0 ;
        sti();
        // reset it to 0 for next wait
    }
    return 0;
    //Instead you set the RTC to the highest possible frequency and wait until you receive
    //x interrupts to return back to the process such that those x interrupts at the highest frequency is equivalent to 1
    //interrupt at the frequency the process wants
}

/*
 * rtc_write
 *   DESCRIPTION: change the virtual frequency
 *   INPUTS: freq
 *   OUTPUTS: none
 *   RETURN VALUE: 1 if success, -1 if get wrong arg
 *   SIDE EFFECTS: change the virtual frequency
 */
int rtc_write(int freq)
{
    // check if the frequency out of range, which is 2 - 1024
    if( freq < 2 || freq > 1024){
        return -1;
    }
    issue.rtc_freq = freq;
    return 1;
}
/*
 * rtc_open
 *   DESCRIPTION: reset the virtual frequency into 2hz
 *   INPUTS: freq
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: reset the virtual frequency into 2hz
 */
int rtc_open()
{
    issue.rtc_freq = 1024; // which is the smallest freq(default)
    return 0;
}

/*
 *   rtc_close
 *   DESCRIPTION: returns success
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int rtc_close()
{
	return 0; //return 0 with success
}
