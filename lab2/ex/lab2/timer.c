#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer < 0 || timer > 2) return 1;
  if(freq < 19 || freq > TIMER_FREQ) return 1;
  
  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) return 1;  //read the config from the timer
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;  //Keep bits 0,1,2 and 3 and set bits 4 and 5 to 11 (LSB followed by MSB)
  
  uint32_t c = TIMER_FREQ/freq; //initial value that will be inside timer

  uint8_t c_lsb, c_msb;
  util_get_LSB(c,&c_lsb);
  util_get_MSB(c,&c_msb);

  uint8_t timer_addy;
  switch(timer){
    case 0:
    controlWord |= TIMER_SEL0;
    timer_addy = TIMER_0;
    break;

  case 1:
    controlWord |= TIMER_SEL1;
    timer_addy = TIMER_1;
    break;

  case 2:
    controlWord |= TIMER_SEL2;
    timer_addy = TIMER_2;
    break;

  default:
    return 1;
  }

  //Insert the constructed control word into the control address
  if (sys_outb(TIMER_CTRL,controlWord) != 0) return 1;

  //Provide address of timer and insert LSB followed bt MSB
  if (sys_outb(timer_addy,c_lsb) != 0) return 1;
  if (sys_outb(timer_addy, c_msb) != 0) return 1;

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


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1; // validação de input
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); // construção do readback command
  if (sys_outb(TIMER_CTRL, RBC) != 0) return 1;       // avisar o i8254 que vamos ler a configuração
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;    // lemos a configuração diretamente do registo associado ao timer
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t conf, enum timer_status_field field) {
  if(timer < 0 || timer > 2) return 1;

  union timer_status_field_val config;

  switch (field) {

    case tsf_all: {
      config.byte = conf;
      break;
    }

    case tsf_initial: {
      conf = (conf >> 4) & 0x03;

      if (conf == 1) {
        config.in_mode = LSB_only;
      }
      else if (conf == 2) {
        config.in_mode = MSB_only;
      }
      else if (conf == 3) {
        config.in_mode = MSB_after_LSB;
      }
      else {
        config.in_mode = INVAL_val;
      }
      break;
    }

    case tsf_mode: {
      conf = (conf >> 1) & 0x07;
      if(conf == 6) config.count_mode = 2;
      else if(conf == 7) config.count_mode = 3;
      else config.count_mode = conf;
      break; 
    }

    case tsf_base: {
      conf = conf & BIT(1);
      config.bcd = conf;
      break;
    }

    default: return 1;
  }

  timer_print_config(timer,field,config);
  return 0;
}
