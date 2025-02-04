#include "game/game.h"

bool isGameRunning;

//EXTERNS 
//game
extern char word[MAX_WORD_LENGTH];
extern GameState gameState;
extern bool gameActive;
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
extern int current_x;
extern int current_y;
extern uint8_t byte_index; 
//timer
extern int counter_timer;
extern int seconds;
//sprites
extern Sprite *mouseCursor;




int handle_interrupts(){

    video_init(0x115);
    loadSprites();

    //set timer frequency to 60hz
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
    if(rtc_subscribe_interrupts()) return 1;

    GameState previousState = gameState;

    while(isGameRunning){
      if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
      }

      if(is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)){
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_timer) {
              timer_int_handler();
              if(counter_timer%60==0 && gameActive){
                seconds++;
              }
              
              if (previousState != gameState) {
                if (gameState == MENU || gameState == SELECT_LETTERS) {
                  vg_exit();
                  video_init(0x115);
                } else if (gameState == GAME_MODE_1 || gameState == GAME_MODE_2) {
                  vg_exit();
                  video_init(0x14C);
                }
                previousState = gameState;
                }

                clear_screen();
                if (gameState == MENU) {
                  draw_menu();
                } else if (gameState == SELECT_LETTERS) {
                  draw_menu2();
                } else if (gameState == GAME_MODE_1 || gameState == GAME_MODE_2) {
                  draw_game();
                }

                drawSprite(mouseCursor, current_x, current_y);
                swap_buffers();
            }
            //Keyboard
            if (msg.m_notify.interrupts & irq_keyboard) {
              kbc_ih();
              if (gameState == MENU) {
                keyboard_handler_menu();
              } else if (gameState == SELECT_LETTERS) {
                keyboard_handler_menu2();
              }  else if (gameState ==  GAME_MODE_1 || gameState == GAME_MODE_2) {
                keyboard_handler_game();
              }
            }
            //mouse
            if (msg.m_notify.interrupts & irq_mouse) {
              mouse_ih();
              sync_mouse();
              if(byte_index == 3){
                mouse_update_packet();
                byte_index = 0;
              }
              if (gameState == MENU || gameState == SELECT_LETTERS) {
                mouse_handler_menu();
              } else if (gameState == GAME_MODE_1 || gameState == GAME_MODE_2) {
                mouse_handler_game();
              }
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
  if (rtc_unsubscribe_interrupts()) return 1;

  free_buffers();
  destroySprites();

  vg_exit();

  return 0;
}

int main(int argc, char **argv)
{
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    //lcf_trace_calls("/home/lcom/labs/g5/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

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
    handle_interrupts();
    return 0;
}

