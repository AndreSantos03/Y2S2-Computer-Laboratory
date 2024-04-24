#ifndef __GAME_H 
#define __GAME_H 

#include <lcom/lcf.h>



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#define MAX_LINE_LENGTH 1024
#define WORDS_AMMOUNT 318
#define MAX_WORD_LENGTH 6

int initialize_game();
int get_word(const char *date);
char* give_guess(const char* guess);

#endif /* __GAME_H */
