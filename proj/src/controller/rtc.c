#include "rtc.h"

int rtc_hook_id = RTC_MASK;
rtc_info info;
uint8_t binary;


int rtc_output(uint8_t command, uint8_t *output){
    if(sys_outb(REGISTER_INPUT,command) != 0) return 1;
        if(util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;

    return 0;
}

int rtc_is_updating(){
    uint8_t res;
    if(rtc_output(UPDATE_STATUS_CMD, &res)) return 1;
        return res & UPDATING;
}

int rtc_is_binary(){
    uint8_t res;
    if(rtc_output(COUNTING_STATUS_CMD, &res)) return 1;
        return res & BINARY;
}

int rtc_is_bcd(){
    return !rtc_is_binary();
}

uint8_t to_binary(uint8_t bcd){
    return ((bcd >> 4) * 10) + (bcd & 0xF);
}

int rtc_subscribe_interrupts() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_update_time(){

    //See if the RTC is busy, if it is we can't update the registers
    if(rtc_is_updating() != 0) return 1;
    uint8_t output;

    //Seconds
    if(rtc_output(SECONDS, &output) != 0) return 1;
    info.seconds = binary ? output : to_binary(output);

    //Minutes
    if(rtc_output(MINUTES, &output) != 0) return 1;
    info.minutes = binary ? output : to_binary(output);

    //Hours
    if(rtc_output(HOURS, &output) != 0) return 1;
    info.hours = binary ? output : to_binary(output);

    //Day
    if(rtc_output(DAY, &output) != 0) return 1;
    info.day = binary ? output : to_binary(output);

    //Month
    if(rtc_output(MONTH, &output) != 0) return 1;
    info.month = binary ? output : to_binary(output);

    //Year
    if(rtc_output(YEAR, &output) != 0) return 1;
    info.year = binary ? output : to_binary(output);

    return 0;
}
