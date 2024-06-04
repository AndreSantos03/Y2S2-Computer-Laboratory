#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define SECONDS 0
#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9
#define UPDATE_STATUS_CMD 10
#define COUNTING_STATUS_CMD 11

#define BINARY BIT(2)
#define UPDATING BIT(7)

#define RTC_MASK            5
#define IRQ_RTC             8
#define REGISTER_INPUT      0x70
#define REGISTER_OUTPUT     0x71

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_info;

int rtc_output(uint8_t command, uint8_t *output);
int rtc_is_updating();
int rtc_is_binary();
int rtc_is_bcd();
uint8_t to_binary(uint8_t bcd);
int rtc_subscribe_interrupts();
int rtc_unsubscribe_interrupts();
int rtc_update_time();
void rtc_setup();

#endif
