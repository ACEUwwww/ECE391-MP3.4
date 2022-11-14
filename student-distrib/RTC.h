#include "types.h"

#define NMI_BIT 0x80 // what?
#define     RTC_CMD    0x70
#define     RTC_CMOS  0x71

#define     RegA              0x8A
#define     RegB              0x8B
#define     RegC              0x8C
#define     RTC_IRQ_index     8 // referred from OSDEV 
#define     bit6              0x40 // used for get bit6
#define     bits4to7          0xF0 // used for get bit4 - 7

typedef struct {
    int rtc_freq;
}issue_info;

issue_info issue;

void rtc_init();
int32_t getRate(int32_t freq);
void rtc_interrupt_handler();
int rtc_open();
int rtc_read(int freq);
int rtc_write(int freq);
int rtc_close();
