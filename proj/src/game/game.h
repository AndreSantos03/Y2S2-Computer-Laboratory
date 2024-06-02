/**
 * @file game.h
 * @brief Game header file, contains declarations.
 * 
 * This file contains all the declarations and includes necessary for the implementations in game.c
 */
#ifndef __GAME_H 
#define __GAME_H 



#include <lcom/lcf.h>
#include "../sprites/sprites.h"
#include "../controller/mouse.h"
#include "../controller/graphics.h"
#include "../controller/keyboard.h"
#include "../controller/i8042.h"
#include "../controller/rtc.h"
#include "../controller/i8254.h"

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

/**
 * @brief Enumeration representing the different states of the game.
 *
 * This enumeration defines the possible states that the game can be in.
 * Each state represents a different phase or mode of the game.
 */
typedef enum {
    MENU,             ///< The game is in the main menu state.
    SELECT_LETTERS,   ///< The game is in the state of selecting letters.
    GAME_MODE_1,      ///< The game is in game mode 1.
    GAME_MODE_2       ///< The game is in game mode 2.
} GameState;

/**
 * @struct WordEntry
 * @brief Struct representing a Word Entry
 *
 * This struct represents an Entry of the word of the day.
 * It contains the date associated to the word, and the word.
 */
typedef struct {
    char date[11];
    char word[MAX_LINE_LENGTH];
} WordEntry;


/**
 * @brief Initializes the game for Game Mode 1.
 *
 * This function resets the game state for Game Mode 1, loads the word list
 * from a CSV file, updates the real-time clock, and selects the word for the
 * current date. It also initializes the game timer.
 *
 * @return 0 on success, 1 on failure.
 */

int initialize_game1();

/**
 * @brief Initializes the game for Game Mode 2.
 *
 * This function resets the game state for Game Mode 2, selects the appropriate
 * word list based on the current word length, shuffles the words, and starts
 * the game timer.
 * 
 * @return 0 on success, 1 on failure
 */
void initialize_game2();

/**
 * @brief Retrieves the word for a given date.
 *
 * This function searches for the word associated with the provided date in the
 * list of entries and sets it as the current word. The word is converted to
 * uppercase.
 *
 * @param date The date string in the format "YYYY-MM-DD".
 * @return 0 on success, 1 if the word for the date is not found or if the date is NULL.
 */
int get_word(const char *date);

/**
 * @brief Processes the current guess.
 *
 * This function compares the current guess with the target word, updates the
 * results array with feedback for each letter, and checks if the game has been
 * won or lost. It also updates the game state accordingly.
 */
void give_guess();

/**
 * @brief Draws the current game state on the screen.
 *
 * This function renders the game board, including the guessed letters, the
 * results (colors), and any dragged letters or hint buttons. It also displays
 * the win/lose message if the game is over.
 *
 * @return 0 on success.
 */
int draw_game();

/**
 * @brief Draws the main menu on the screen.
 *
 * This function renders the main menu options and highlights the option that
 * the mouse is hovering over. It also displays instructions for selecting an option.
 *
 * @return 0 on success.
 */
int draw_menu();

/**
 * @brief Draws the secondary menu for selecting the number of letters per word.
 *
 * This function renders the options for selecting the number of letters per word
 * and highlights the option that the mouse is hovering over.
 *
 * @return 0 on success.
 */
int draw_menu2();

/**
 * @brief Draws the on-screen keyboard and mouse.
 *
 * This function renders the visual representation of the keyboard and mouse,
 * including the positions of the letters and any dragged letters.
 */
void draw_mouse_keyboard();

/**
 * @brief Handles mouse input in the main menu.
 *
 * This function processes mouse clicks in the main menu and transitions to the
 * appropriate game state based on the selected option.
 */
void mouse_handler_menu();

/**
 * @brief Handles mouse input during the game.
 *
 * This function processes mouse clicks and drags during the game, allowing the
 * player to select letters from the visual keyboard and place them in the current
 * guess. It also handles clicks on the "Restart" button and the hint button.
 */
void mouse_handler_game();

/**
 * @brief Handles keyboard input during the game.
 *
 * This function processes keyboard input, allowing the player to submit guesses
 * (Enter), delete letters (Backspace), and return to the menu or start a new game
 * (Escape or Enter after game over).
 */
void keyboard_handler_game();

/**
 * @brief Handles keyboard input in the main menu.
 *
 * This function processes keyboard input in the main menu and exits the game
 * if the Escape key is pressed.
 */
void keyboard_handler_menu();

/**
 * @brief Handles keyboard input in the secondary menu.
 *
 * This function processes keyboard input in the secondary menu and returns to
 * the main menu if the Escape key is pressed.
 */
void keyboard_handler_menu2();

/**
 * @brief Shuffles an array of words.
 *
 * This function randomly shuffles an array of words using the Fisher-Yates
 * shuffle algorithm.
 *
 * @param words An array of word strings to shuffle.
 * @param count The number of words in the array.
 */
void shuffle_words(char *words[], int count);

/**
 * @brief Draws the hint button on the screen.
 *
 * This function renders the hint button at a fixed position on the screen. If
 * the hint button is being dragged, it also renders the dragging indicator.
 */
void draw_hint_button();

#endif
