// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#include "i8042.h"
#include "timer.c"

extern struct packet mouse_packet;
extern uint8_t byte_index ; 
extern int counter;



int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;

  uint8_t irq_set;

  if (mouse_subscribe_interrupts(& irq_set) != 0) return 1;
  
  //enable data reporting
  if (write_mouse(ENABLE_DATA_REP) != 0) return 1;

  while(cnt > 0){
    if( driver_receive(ANY, &msg, &ipc_status) != 0){
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if(msg.m_notify.interrupts & irq_set){
            mouse_ih();
            sync_mouse();
            if(byte_index == 3){
              mouse_update_packet();
              mouse_print_packet(&mouse_packet);
              //go back to the first byte
              byte_index = 0;
              cnt--;
            }
          }
      }
    }
  }

  //DONT FORGET
  // disable the data report now
  if (write_mouse(DISABLE_DATA_REP) != 0) return 1;


  if (mouse_unsubscribe_interrupts() != 0) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  message msg;

  uint8_t irq_set_mouse, irq_set_timer;
  uint8_t passed_time = 0;

  int frequency = sys_hz();

  if (mouse_subscribe_interrupts(& irq_set_mouse) != 0) return 1;
  if (timer_subscribe_int(& irq_set_timer) != 0) return 1;

  //enable data reporting
  if (write_mouse(ENABLE_DATA_REP) != 0) return 1;

  while(idle_time >passed_time){
    if( driver_receive(ANY, &msg, &ipc_status) != 0){
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          //mouse
          if(msg.m_notify.interrupts & irq_set_mouse){
            mouse_ih();
            sync_mouse();
            if(byte_index == 3){
              mouse_update_packet();
              mouse_print_packet(&mouse_packet);
              //go back to the first byte
              byte_index = 0;
            }
            passed_time = 0;
            counter = 0;
          }
          //timer
            if (msg.m_notify.interrupts & irq_set_timer) { // Se for uma interrupão do timer
              timer_int_handler();
            if (counter % frequency == 0) {
              passed_time++;
            }
        }
      }
    }
  }

  //DONT FORGET
  // disable the data report now
  if (write_mouse(DISABLE_DATA_REP) != 0) return 1;


  if (mouse_unsubscribe_interrupts() != 0) return 1;
  if (timer_unsubscribe_int()!= 0) return 1;

  return 0;

}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
