#include <lcom/lcf.h>
#include "controller/graphics.h"
#include "controller/mouse.h"
#include "controller/keyboard.h"


bool isGameRunning;

//EXTERNS 
//video
extern vbe_mode_info_t mode_info;
extern int bytesPerPixel;
extern int xResolution;
extern int yResolution;
extern int bufferSize;
extern uint8_t *mainBuffer;
//keyboard
extern uint8_t scancode;
//mouse
extern struct packet mouse_packet;
extern uint8_t mouse_bytes[3];
extern int current_x;
extern int current_y;
//timer
extern int counter_timer;




int handle_interrupts(){

    //set timer frequency to 80hz
    if(timer_set_frequency(0, 60)) return 1;

    // Stream mode, writting command
    if (write_mouse(0xEA)) return 1; 
    // Data Report
    if (write_mouse(0xF4)) return 1; 

    int ipc_status;
    uint8_t irq_keyboard, irq_mouse, irq_timer;
    message msg;


    //SUBSCRIBES
    if(keyboard_subscribe_interrupts(&irq_keyboard)) return 1;

    if(mouse_subscribe_interrupts(&irq_mouse)) return 1;

    if(timer_subscribe_int(&irq_timer)) return 1;

    while(isGameRunning && scancode != BREAK_ESC){
      if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
      }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            timer_int_handler();
            printf("passed %d",counter_timer);
          }
          //Keyboard
          if (msg.m_notify.interrupts & irq_keyboard) {
            kbc_ih();
          }
          //mouse
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            sync_mouse();
          }
          break;
        default:
          break;
      }
    }
  }

  //UNSUBSCRIBES
    if(keyboard_unsubscribe_interrupts()) return 1;

    if(mouse_unsubscribe_interrupts()) return 1;

    if(timer_unsubscribe_int()) return 1;

    return 0;
}

int main(int argc, char **argv)
{
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/g4/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/g4/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv)) return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();
    return 0;
}

int (proj_main_loop)(int argc, char **argv) {
    isGameRunning = true;
    printf("we got a project baby!\n");
    handle_interrupts();
    return 0;
}
