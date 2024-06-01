#include "game.h"

//externs
extern bool isGameRunning; //Meaning the program as a whole
extern int xResolution;
extern int yResolution;
//mouse
extern int current_x;
extern int current_y;
extern struct packet mouse_packet;

//keyboard
extern uint8_t scancode;

//sprites
extern Sprite *letterSprites[26];

char word[MAX_WORD_LENGTH];
char attempts[GUESS_ATTEMPTS][MAX_WORD_LENGTH] = {{'\0'}};
char results[GUESS_ATTEMPTS][MAX_WORD_LENGTH] = {{'\0'}};

int current_guess = 0; //This used to be current_word but I found it more fitting to name it like this because current_word fits for game mode 2
int current_word = 0;
int current_word_length = 0;
int current_letter = 0;
int currentCount = 0;

char *words3[] = {"CAT", "DOG", "BAT", "HAT", "RAT"};
char *words4[] = {"TREE", "LION", "BEAR", "FROG", "FISH"};
char *words5[] = {"HOUSE", "APPLE", "GRAPE", "TABLE", "CHAIR"};
char *words6[] = {"ORANGE", "BANANA", "CHERRY", "PEACH", "BOTTLE"};

int words3_count = sizeof(words3) / sizeof(words3[0]);
int words4_count = sizeof(words4) / sizeof(words4[0]);
int words5_count = sizeof(words5) / sizeof(words5[0]);
int words6_count = sizeof(words6) / sizeof(words6[0]);

char **currentWords;
int currentWordsCount;

typedef struct {
    char date[11];
    char word[MAX_LINE_LENGTH];
} WordEntry;

WordEntry entries[WORDS_AMMOUNT];

bool gameActive = false; //Meaning a game instance
GameState gameState = MENU;
bool gameWon = false;
bool game2Won = false;

char* menuOptions[MENU_OPTIONS] = {"GAME MODE I", "GAME MODE II"};
char* menu2Options[MENU2_OPTIONS] = {"III", "IV", "V", "VI"};
int selectedOption = -1;
bool optionSelected = false;


int initialize_game1(){

    memset(attempts, '\0', sizeof(attempts));
    memset(results, '\0', sizeof(results));

    gameWon = false;
    current_guess = 0;
    current_letter = 0;

    //TODO there must be a better way to do this
    FILE *file = fopen("/home/lcom/labs/proj/src/game/words.csv", "r"); //Tive de tirar '/g5' pq n tava a dar para mim, mesma coisa na main()
    if (file == NULL) {
        printf("Error opening file: %s\n", "words.csv");
        return 1;
    }

    char header[MAX_LINE_LENGTH];
    fgets(header, MAX_LINE_LENGTH, file);

    int index = 0; 
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && index < 366) {
        char date[11];
        char word[MAX_LINE_LENGTH];
        sscanf(line, "%10[^,],%[^,\n]", date, word);
        strcpy(entries[index].date, date);
        strcpy(entries[index].word, word);
        index++;
    }

    fclose(file);

    if(rtc_update_time() != 0){
        printf("Failed to update RTC time \n");
        return 1;
    }

    //Format the data
    char date_str[11];
    snprintf(date_str,11,"%04u-%02u-%02u",2000 + info.year, info.month, info.day);

    if(get_word(date_str) != 0){
        printf("Failed to get word for date: %s\n", date_str);
        return 1;
    }

    gameActive = true;
    return 0;
}


void initialize_game2() {

    memset(attempts, '\0', sizeof(attempts));
    memset(results, '\0', sizeof(results));

    gameWon = false;
    current_guess = 0;
    current_letter = 0;

    switch(current_word_length) {
        case 3:
            currentWords = words3;
            currentWordsCount = words3_count;
            break;
        case 4:
            currentWords = words4;
            currentWordsCount = words4_count;
            break;
        case 5:
            currentWords = words5;
            currentWordsCount = words5_count;
            break;
        case 6:
            currentWords = words6;
            currentWordsCount = words6_count;
            break;
        default:
            printf("Invalid word length\n");
            return;
    }

    if (!gameActive) {
        shuffle_words(currentWords, currentWordsCount);
    }

    gameActive = true;
    strcpy(word, currentWords[current_word]);
}


int get_word(const char *date) {
    printf("inside\n");
    if (date == NULL) {
        printf("Invalid argument: date is NULL.\n");
        return 1;
    }

    int found = 0;
    for (int i = 0; i < WORDS_AMMOUNT; i++) {
        if (strcmp(entries[i].date, date) == 0) {
            strcpy(word, entries[i].word);

            // Convert all characters in word to uppercase
            for (int j = 0; word[j] != '\0'; j++) {
                word[j] = toupper((unsigned char)(word[j]));
            }

            word[current_word_length] = '\0'; 
            found = 1;
            break;

        }
    }
    if (!found) {
        printf("Word not found for date: %s\n", date);
        return 1; 
    }

    return 0; 
}


void give_guess() {
    
    if(!gameActive) return;

    //create letter frequency table
    int letterFrequency[26] = {0};
    for (int i = 0; word[i]; i++) {
        letterFrequency[word[i] - 'A']++;
    }

    int currentCount = 0;
    for (int i = 0; word[i]; i++) {
        char charGuess = attempts[current_guess][i];
        char charWord = word[i];
        printf(" char guess %c   and charWord %c\n",charGuess,charWord);
        char color;
        if (charGuess == charWord) {
            color = 'G';
            letterFrequency[charGuess - 'A']--;
            currentCount++;
        } else if (letterFrequency[charGuess - 'A'] > 0) {
            color = 'Y';
            letterFrequency[charGuess - 'A']--;
        } else {
            color = 'R';
        }
        results[current_guess][i] = color;
    }

    if(currentCount == current_word_length) {
        gameActive = false;
        gameWon = true;
        if (gameState == GAME_MODE_2) {
            if (current_word == currentWordsCount) {
                game2Won = true;
            }
        }
    } else if (current_guess == GUESS_ATTEMPTS-1) {
        gameActive = false;
        gameWon = false;
    } else {
        current_guess++;
    }
    current_letter = 0;
}

int draw_game(){
    int spaceBetweenWords = ((yResolution * 0.8)/ GUESS_ATTEMPTS );//splits the screen between attempts and leaves 0.2 of the bottom empty

    //draw guess attempt
    for(int row = 0; row < GUESS_ATTEMPTS;row++){

        int yPos = spaceBetweenWords * row + 10; //10 is the offset for the beggining

        //draw line
        int spaceBetweenLetters = (xResolution/(2 * current_word_length));
        for (int col = 0; col < current_word_length; col++) {

            int xPos = (xResolution / 4) + (spaceBetweenLetters * col);

            char letter = attempts[row][col];
            char colorResult = results[row][col];

            //checks to see if result has been given
            if(colorResult != '\0'){
                uint32_t colorHex;
                switch (colorResult)
                {
                case 'G':
                    colorHex = 0x00FF00; // Green color
                    break;
                case 'R':
                    colorHex = 0xFF0000; // Red color
                    break;
                case 'Y':
                    colorHex = 0xFFFF00; // Yellow color
                    break;
                default:
                    colorHex = 0x000000; // Black color
                    break;
                }
                draw_rectangle(xPos-8,yPos-2,BORDER_WIDTH,BORDER_HEIGHT,colorHex);
            }



            //checks to see if the attempts has been written
            if(letter != '\0'){
                int index = attempts[row][col] - 'A';

                Sprite *letterSprite = letterSprites[index];
                //draw the letter
                drawSprite(letterSprite, xPos, yPos);
            }

            //draw the rectangle box
            draw_border(xPos-8,yPos-2,BORDER_WIDTH,BORDER_HEIGHT,0xFFFFFF,3);        
        }
    }

    // Draw the win/lose message if the game is over
    if (!gameActive) {
        clear_screen();
        if (game2Won) {
            drawText("YOU WON THIS GAME MODE", (xResolution - strlen("YOU WON THIS GAME MODE") * letterSprites[0]->width) / 2, yResolution - letterSprites[0]->height - 300);
        } else if (gameWon) {
            drawText("YOU WON", (xResolution - 8 * letterSprites[0]->width) / 2, yResolution - letterSprites[0]->height - 300);
        } else {
            drawText("YOU LOSE", (xResolution - 9 * letterSprites[0]->width) / 2, yResolution - letterSprites[0]->height - 300);
            drawText(word, (xResolution - strlen(word) * letterSprites[0]->width) / 2, yResolution - 2 * letterSprites[0]->height - 150);
        } //deviamos de mostrar a palavra no game mode 2?
        drawText("PRESS ENTER TO CONTINUE", (xResolution - strlen("PRESS ENTER TO CONTINUE") * letterSprites[0]->width) / 2, yResolution - letterSprites[0]->height - 10);
        return 0;
    }

    int buttonWidth = 200;
    int buttonHeight = 50;
    int xPosButton = (xResolution - buttonWidth) / 2;
    int yPosButton = yResolution - buttonHeight - 20;
    draw_rectangle(xPosButton-10, yPosButton, buttonWidth + 16, buttonHeight, 0xFF0000); // Red background for button
    drawText("RESTART", (xResolution - strlen("RESTART") * letterSprites[0]->width) / 2, yPosButton + (buttonHeight - letterSprites[0]->height) / 2);

    return 0;
}

int draw_menu() {
    int spaceBetweenOptions = (yResolution / (MENU_OPTIONS + 2)); // Space between options, with some padding
    int optionWidth, optionHeight;
    optionSelected= false;

    // Draw menu options
    for (int i = 0; i < MENU_OPTIONS; i++) {
        int xPos = (xResolution - strlen(menuOptions[i]) * letterSprites[0]->width) / 2;
        int yPos = spaceBetweenOptions * (i + 1);

        optionWidth = strlen(menuOptions[i]) * letterSprites[0]->width;
        optionHeight = letterSprites[0]->height;

        if (current_x >= xPos && current_x <= xPos + optionWidth && current_y >= yPos && current_y <= yPos + optionHeight) {
            // Highlight the option if the mouse is hovering over it
            draw_rectangle(xPos - 10, yPos - 10, optionWidth + 20, optionHeight + 20, 0xFFFF00); // Yellow background for selection
            selectedOption = i; // Update the selected option based on mouse position
            optionSelected = true;
        }

        if (!optionSelected) {
            selectedOption = -1;
        }

        drawText(menuOptions[i], xPos, yPos); // Draw the menu option text
    }

    // Display instructions
    char *instructions = "CLICK TO SELECT AN OPTION";
    int xPosInstructions = (xResolution - strlen(instructions) * letterSprites[0]->width) / 2;
    int yPosInstructions = yResolution - spaceBetweenOptions;
    drawText(instructions, xPosInstructions, yPosInstructions);

    return 0;

}

int draw_menu2() {
    int spaceBetweenOptions = (yResolution / (4+2)); //4 is the number of options
    int optionWidth, optionHeight;
    optionSelected = false;

    // Draw menu title
    char *title = "LETTERS PER WORD";
    int xPosTitle = (xResolution - strlen(title) * letterSprites[0]->width) / 2;
    int yPosTitle = spaceBetweenOptions;
    drawText(title, xPosTitle, yPosTitle);


    for (int i = 0; i < 4; i++) {
        int xPos = (xResolution - strlen(menu2Options[i]) * letterSprites[0]->width) / 2;
        int yPos = spaceBetweenOptions * (i + 2);

        optionWidth = strlen(menu2Options[i]) * letterSprites[0]->width;
        optionHeight = letterSprites[0]->height;

        if (current_x >= xPos && current_x <= xPos + optionWidth && current_y >= yPos && current_y <= yPos + optionHeight) {
            draw_rectangle(xPos - 10, yPos - 10, optionWidth + 20, optionHeight + 20, 0xFFFF00); 
            selectedOption = i + 3;
            optionSelected = true;
        }
    }

    if (!optionSelected) {
        selectedOption = -1;
    }
    

    // Draw menu options text
    for (int i = 0; i < 4; i++) {
        int xPos = (xResolution - strlen(menu2Options[i]) * letterSprites[0]->width) / 2;
        int yPos = spaceBetweenOptions * (i + 2);

        drawText(menu2Options[i], xPos, yPos); // Draw the menu option text
    }

    return 0;
}


void mouse_handler_menu() {
    if (mouse_packet.lb) {
        if (selectedOption == 0) {
            gameState = GAME_MODE_1;
            current_word_length = 5;
            initialize_game1();
        } else if (selectedOption == 1) {
            gameState = SELECT_LETTERS;
        } else if (selectedOption >= 3 && selectedOption <= 6) {
            gameState = GAME_MODE_2;
            current_word_length = selectedOption;
            initialize_game2();
        }
    }
}


void mouse_handler_game() {
    if (mouse_packet.lb) {
        if (!gameWon) {
            // Check if the mouse click is within the "Restart" button bounds
            int buttonWidth = 200;
            int buttonHeight = 50;
            int xPosButton = (xResolution - buttonWidth) / 2;
            int yPosButton = yResolution - buttonHeight - 20;

            if (current_x >= xPosButton - 10 && current_x <= xPosButton + buttonWidth + 16 &&
                current_y >= yPosButton && current_y <= yPosButton + buttonHeight) {
                if (gameState == GAME_MODE_1) {
                    initialize_game1();
                } else if (gameState == GAME_MODE_2) {
                    initialize_game2();
                }
            }
        }
    }
}

void keyboard_handler_game(){

    if(!gameActive) {
        if (game2Won) {
            if (scancode == BREAK_ENTER) {
                if (gameState == GAME_MODE_2) {
                    gameState = MENU;
                }
            }
        } else if (gameWon) {
            if (scancode == BREAK_ENTER) {
                switch (gameState) {
                    case GAME_MODE_1: {
                        gameState = MENU;
                    }
                    case GAME_MODE_2: {
                        current_word++;
                        initialize_game2();
                    }
                    default: break;
                }
            }
        } else {
            if (scancode == BREAK_ENTER) {
                switch (gameState) {
                    case GAME_MODE_1: {
                        gameState = MENU;
                    }
                    case GAME_MODE_2: {
                        current_word = 0;
                        initialize_game2();
                    }
                    default: break;
                }
            }
        }
        
    }

    //backspace
    if(scancode == BREAK_BACK){
        if(current_letter != 0) current_letter--;
        attempts[current_guess][current_letter] = '\0';
    }

    //enter
    if(scancode == BREAK_ENTER){
        if(current_letter != current_word_length)return;
        give_guess();
    }

    if (scancode == BREAK_ESC) {
        gameActive = false;
        gameState = MENU;
    }

    //checks to see if the keyboard input is a letter breakcode
    int letter_index = get_letter_index(scancode);
    if(letter_index != -1){
        //check to see if its filled
        if(current_letter == current_word_length) return;
        char letter = 'A' + letter_index;
        attempts[current_guess][current_letter] = letter;
        current_letter++;
    }

}

void keyboard_handler_menu() {
    if (scancode == BREAK_ESC) {
        isGameRunning = false; //Ends the while loop in main.cpp
    }
}

void keyboard_handler_menu2() {
    if (scancode == BREAK_ESC) {
        gameState = MENU;  
    }
}

void shuffle_words(char *words[], int count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = words[i];
        words[i] = words[j];
        words[j] = temp;
    }
}
