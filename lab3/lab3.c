#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <i8042.h>

#include "keyboard.h"

extern uint8_t scancode;
extern uint32_t sys_calls_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(kbd_test_scan)() {
  int ipc_status;
  message msg;

  uint8_t irq_set;
  if( keyboard_subscribe_interrupts(&irq_set) ) return 1;


  while(scancode != BREAK_ESC){
    if( driver_receive(ANY, &msg, &ipc_status) != 0){
        continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if(msg.m_notify.interrupts & irq_set){
            kbc_ih();
            kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
          }
      }
    }
  }
  
  if (keyboard_unsubscribe_interrupts() != 0) return 1;

  if (kbd_print_no_sysinb(sys_calls_counter) != 0) return 1;

  return 0;
}



int(kbd_test_poll)() {
  while (scancode != BREAK_ESC){
    if(read_keyboard_output(KBD_OUT_BUF,&scancode) == 0 ){
      kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
    }
  }

  return keyboard_restore();
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
