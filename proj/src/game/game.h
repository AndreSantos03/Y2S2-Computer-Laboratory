#ifndef __GAME_H 
#define __GAME_H 

#include <lcom/lcf.h>
#include "../sprites/sprites.h"
#include "../controller/mouse.h"
#include "../controller/graphics.h"
#include "../controller/keyboard.h"
#include "../controller/i8042.h"
#include "../controller/rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>


#define MAX_LINE_LENGTH 1024
#define WORDS_AMMOUNT 318
#define MAX_WORD_LENGTH 6
#define GUESS_ATTEMPTS 5
#define MENU_OPTIONS 2
#define MENU2_OPTIONS 4

#define BORDER_WIDTH 45
#define BORDER_HEIGHT 50

typedef enum {
    MENU,
    SELECT_LETTERS,
    GAME_MODE_1,
    GAME_MODE_2
} GameState;


int initialize_game1();
void initialize_game2();
int get_word(const char *date);
void give_guess();
int draw_game();
int draw_menu();
int draw_menu2();
void draw_mouse_keyboard();
void draw_hint_button();
void mouse_handler_menu();
void mouse_handler_game();
void keyboard_handler_game();
void keyboard_handler_menu();
void keyboard_handler_menu2();
void shuffle_words(char *words[], int count);


#endif
