#include "rtc.h"

int rtc_hook_id = RTC_MASK;
rtc_info info;
uint8_t binary;

int rtc_subscribe_interrupts() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_output(uint8_t command, uint8_t *output){
    if(sys_outb(REGISTER_INPUT,command) != 0) return 1;
        if(util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;

    return 0;
}

int rtc_is_updating(){
    uint8_t res;
    if(rtc_output(REGISTER_UPDATING, &result)) return 1;
        return result & UPDATING;
}