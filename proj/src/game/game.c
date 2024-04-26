#include <lcom/lcf.h>

#include "game.h"

//externs
extern int xResolution;
extern int yResolution;
//mouse
extern int current_x;
extern int current_y;

//keyboard
extern uint8_t scancode;

//sprites
extern Sprite *letterSprites[26];

char word[MAX_WORD_LENGTH];
char attempts[GUESS_ATTEMPTS][MAX_WORD_LENGTH] = {{'\0'}};
char results[GUESS_ATTEMPTS][MAX_WORD_LENGTH] = {{'\0'}};

int current_word = 0;
int current_letter = 0;

typedef struct {
    char date[11];
    char word[MAX_LINE_LENGTH];
} WordEntry;

WordEntry entries[WORDS_AMMOUNT];




int initialize_game(){

    //TODO there must be a better way to do this
    FILE *file = fopen("/home/lcom/labs/g5/proj/src/game/words.csv", "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", "dadwadwa");
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

    return 0;
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

            word[MAX_WORD_LENGTH - 1] = '\0'; 
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
    

    //create letter frequency table
    int letterFrequency[26] = {0};
    for (int i = 0; word[i]; i++) {
        letterFrequency[word[i] - 'A']++;
    }

    for (int i = 0; word[i]; i++) {
        char charGuess = attempts[current_word][i];
        char charWord = word[i];
        printf(" char guess %c   and charWord %c\n",charGuess,charWord);
        char color;
        if (charGuess == charWord) {
            color = 'G';
            letterFrequency[charGuess - 'A']--;
        } else if (letterFrequency[charGuess - 'A'] > 0) {
            color = 'Y';
            letterFrequency[charGuess - 'A']--;
        } else {
            color = 'R';
        }
        results[current_word][i] = color;
    }
}

int draw_game(){
    int spaceBetweenWords = ((yResolution * 0.8)/ GUESS_ATTEMPTS );//splits the screen between attempts and leaves 0.2 of the bottom empty


    //draw guess attempt
    for(int row = 0; row < GUESS_ATTEMPTS;row++){


        int yPos = spaceBetweenWords * row + 10; //10 is the offset for the beggining

        //draw line
        int spaceBetweenLetters = (xResolution/(2 * (MAX_WORD_LENGTH - 1)));
        for (int col = 0; col < MAX_WORD_LENGTH - 1; col++) {

            int xPos = (xResolution / 4) + (spaceBetweenLetters * col);

            char letter = attempts[row][col];
            char colorResult = results[row][col];

            //checks to see if result has been given
            if(colorResult != '\0'){
                uint32_t colorHex;
                switch (colorResult)
                {
                case 'G':
                    colorHex = 0x00FF00;
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



            //checks to see if the the attempts has been written
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
    return 0;
}

void keyboard_handler_game(){ 
    //backspace
    if(scancode == BREAK_BACK){
        if(current_letter != 0) current_letter--;


        attempts[current_word][current_letter] = '\0';
    }

    //enter
    if(scancode == BREAK_ENTER){
        if(current_letter != MAX_WORD_LENGTH - 1)return;
        give_guess();
        current_word++;
        current_letter = 0;
    }

    //checks to see if the keyboard input is a letter breakcode
    int letter_index = get_letter_index(scancode);
    if(letter_index != -1){
        //check to see if its filled
        if(current_letter == MAX_WORD_LENGTH - 1) return;


        char letter = 'A' + letter_index;
        attempts[current_word][current_letter] = letter;
        current_letter++;
    }
}

