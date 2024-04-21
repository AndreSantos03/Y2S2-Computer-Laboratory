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
extern uint8_t mouse_bytes[3];
extern int current_x;
extern int current_y;



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

typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
  } GestureState;

GestureState state = START;


int first_x;
int first_y;
int vertex_x;
int vertex_y;

void state_machine_handler(uint8_t x_len,uint8_t tolerance){
  switch (state) {

      case START:
          if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
            state = UP;
            first_x = current_x;
            first_y = current_y;    

          }

          break;

      case UP:
          if(!mouse_packet.lb){


            //save vertex position
            vertex_x = current_x;
            vertex_y = current_y;

            //checks the x len
            if(vertex_x - first_x < x_len){
              state = START;
            }
            else{
              state = VERTEX;
            }
          } 
          else{


            // Check if the error passes the tolerance
            if (mouse_packet.delta_x < 0 && abs(mouse_packet.delta_x) > tolerance) {
                state = START;
            } else if (mouse_packet.delta_y < 0 && abs(mouse_packet.delta_y) > tolerance) {
                state = START;  
            }

            //else we continue in state UP
          }
          break;
      case VERTEX:
          if(mouse_packet.lb){
            first_x = current_x;
            first_y = current_y;
            state = UP;
          }
          else if(mouse_packet.rb){
            //check if the vertex displacement
            if(abs(vertex_x - current_x) > tolerance || abs(vertex_y - current_y) > tolerance) {
              state = START;
            }
            else{
              state = DOWN;

            }
          }
          break;

      case DOWN:
        if(!mouse_packet.rb){
            //checks for xlen
            if(current_x - vertex_x < x_len){
              state = START;
            }
            else{
              state = END;
            }
          } 
          else{

            //check if the error passes the tolerance
            if(mouse_packet.delta_x < 0 && abs(mouse_packet.delta_x) > tolerance){
              state = START;
            }
            else if(mouse_packet.delta_y > 0 && abs(mouse_packet.delta_y) > tolerance){
              state = START;  
            }
          }
          break;
      case END:
          break;
    }
}


const char* state_to_string(GestureState state) {
    switch(state) {
        case START:
            return "START";
        case UP:
            return "UP";
        case VERTEX:
            return "VERTEX";
        case DOWN:
            return "DOWN";
        case END:
            return "END";
        default:
            return "UNKNOWN";
    }
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;

  uint8_t irq_set;

  if (mouse_subscribe_interrupts(& irq_set) != 0) return 1;
  
  //enable data reporting
  if (write_mouse(ENABLE_DATA_REP) != 0) return 1;

  while(state !=END){
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
              state_machine_handler(x_len,tolerance);
              mouse_print_packet(&mouse_packet);
              //go back to the first byte
              byte_index = 0;
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
