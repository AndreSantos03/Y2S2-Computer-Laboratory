// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphics.h"
#include "keyboard.h"
#include "i8042.h"

extern uint8_t scancode;

extern vbe_mode_info_t mode_info;
extern int bytesPerPixel;
extern int xResolution;
extern int yResolution;
extern int bufferSize;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if (vbe_set_mode(mode) != 0) 
        return 1;

    sleep(delay);

    if(vg_exit() != 0)
        return 1;


    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {


    if(video_init(mode) != 0 ) return 1;

    normalize_color(&color);

    if (draw_rectangle(x, y, width, height, color) != 0) {
        printf("fucking error man god damn!\n");
        return 1;
    }
    

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
                    break;
                }
            }
        }
    }
    
    if (keyboard_unsubscribe_interrupts() != 0) return 1;

    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

    if(video_init(mode) != 0 ) return 1;

    int numPixX = xResolution / no_rectangles;
    int numPixY = yResolution / no_rectangles;

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {
      uint32_t color;
      if (mode_info.MemoryModel == DIRECT_COLOR) {

        uint32_t R = Red(j, step, first);
        uint32_t G = Green(i, step, first);
        uint32_t B = Blue(j, i, step, first);
        color = (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
      } 
      else{
        color = (first + (i * no_rectangles + j) * step) % (1 << mode_info.BitsPerPixel);
      }

      if (draw_rectangle(j * numPixX, i * numPixY, numPixX, numPixY, color)) 
        return 1;

    }
  }

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
                    break;
                }
            }
        }
    }
    
    if (keyboard_unsubscribe_interrupts() != 0) return 1;

    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

    if(video_init(0x105) != 0 ) return 1;

    draw_xpm(xpm,x,y);

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
                    break;
                }
            }
        }
    }
    
    if (keyboard_unsubscribe_interrupts() != 0) return 1;

    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
    if(video_init(0x105) != 0 ) return 1;

    bool vertical;
    if (xi == xf && yi < yf) vertical = true;
    else if (yi == yf && xi < xf) vertical = false;
    else return 1;
    int ipc_status;
    message msg;

    uint8_t irq_set_timer,irq_set_keyboard;

    if( keyboard_subscribe_interrupts(&irq_set_keyboard) ) return 1;
    if (timer_subscribe_int(&irq_set_timer) != 0) return 1;

    if (timer_set_frequency(0, fr_rate) != 0) return 1;   


    while(scancode != BREAK_ESC && (xi < xf || yi < yf)){
        if( driver_receive(ANY, &msg, &ipc_status) != 0){
            continue;
        }
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                if(msg.m_notify.interrupts & irq_set_keyboard){
                    kbc_ih();
                    break;
                }
                if(msg.m_notify.interrupts & irq_set_timer){
                    //deletes formet movement
                    if (draw_rectangle(xi, yi, 100, 100, 0xFFFFFF) != 0) return 1;

                    if (vertical) {
                        yi += speed;
                        if (yi > yf) yi = yf;
                    } else {
                        xi += speed;
                        if (xi > xf) xi = xf;
                    }

                    //draw new square
                    if(draw_xpm(xpm, xi, yi) != 0) return 1;

                    break;
                }
            }
        }
    }
    
    if (keyboard_unsubscribe_interrupts() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;


    if (vg_exit() != 0) return 1;

    return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
