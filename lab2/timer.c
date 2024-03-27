#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = 0;

int timer_set_frequency(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2)
    return 1;
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;

  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0)
    return 1;

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;

  uint32_t c = TIMER_FREQ / freq;

  uint8_t control_LSB, control_MSB;
  util_get_LSB(c, &control_LSB);
  util_get_MSB(c, &control_MSB);

  uint8_t timer_register;

  if (timer == 0) {
    timer_register = TIMER_0;
    controlWord |= TIMER_SEL0;
  } else if (timer == 1) {
    timer_register = TIMER_1;
    controlWord |= TIMER_SEL1;
  } else if (timer == 2) {
    timer_register = TIMER_2;
    controlWord |= TIMER_SEL2;
  } else {
    return 1;
  }

  if (sys_outb(timer_register, control_LSB) != 0) {
    return 1;
  }

  if (sys_outb(timer_register, control_MSB) != 0) {
    return 1;
  }

  if (sys_outb(TIMER_CTRL, controlWord) != 0) {
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int timer_get_conf(uint8_t timer, uint8_t *st) {
    if (st == NULL || timer > 2) {
        return 1;
    }

    uint8_t RBC = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, RBC) != 0) {
        return 1;
    }

    if (util_sys_inb(TIMER_0 + timer, st)) {
        return 1;
    }

    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t conf, enum timer_status_field field) {
  if(timer < 0 || timer > 2) return 1;
  union timer_status_field_val timerConfig;

  if(field == tsf_all){
    timerConfig.byte = conf;
  }
  else if(field == tsf_initial){
    conf = (conf >> 4);
    conf = conf & 0x03; //in order to make everything 0 and keep the last 2 bits of conf as is
    if (conf == 1) {
      timerConfig.in_mode = LSB_only;
    }
    else if (conf == 2) {
      timerConfig.in_mode = MSB_only;
    }
    else if (conf == 3) {
      timerConfig.in_mode = MSB_after_LSB;
    }
    else {
      timerConfig.in_mode = INVAL_val;
    }
  }
  else if(field == tsf_mode){
    conf = conf >> 1;
    conf = conf & 0x07;
    timerConfig.count_mode = conf;
  }
  else if (field == tsf_base){
    conf = conf & BIT(1);
    timerConfig.bcd = conf;
  }
  else{
    return 1;
  }
  timer_print_config(timer,field,timerConfig);
  return 0;
}
