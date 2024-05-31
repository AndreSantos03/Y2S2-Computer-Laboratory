#ifndef __GAME_H 
#define __GAME_H 

#include <lcom/lcf.h>
#include "../sprites/sprites.h"
#include "../controller/graphics.h"
#include "../controller/keyboard.h"
#include "../controller/i8042.h"
#include "../controller/rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#define MAX_LINE_LENGTH 1024
#define WORDS_AMMOUNT 318
#define MAX_WORD_LENGTH 6
#define GUESS_ATTEMPTS 5

#define BORDER_WIDTH 45
#define BORDER_HEIGHT 50


int initialize_game();
int get_word(const char *date);
void give_guess();
int draw_game();
void keyboard_handler_game();
#endif /* __GAME_H */
