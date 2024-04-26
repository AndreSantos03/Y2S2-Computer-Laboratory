#include <lcom/lcf.h>

#include "game.h"

//externs
extern int xResolution;
extern int yResolution;
//mouse
extern int current_x;
extern int current_y;
//sprites
extern Sprite *letterSprites[26];

char word[MAX_WORD_LENGTH];

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


char* give_guess(const char* guess) {
    char* result = (char*)malloc(MAX_WORD_LENGTH * sizeof(char));
    
    char upperCurrentWord[MAX_WORD_LENGTH];
    char upperGuess[MAX_WORD_LENGTH];

    strcpy(upperCurrentWord, word);
    strcpy(upperGuess, guess);
    for (int i = 0; upperCurrentWord[i]; i++) {
        upperCurrentWord[i] = toupper((unsigned char)upperCurrentWord[i]);
    }
    for (int i = 0; upperGuess[i]; i++) {
        upperGuess[i] = toupper((unsigned char)upperGuess[i]);
    }

    printf("%s upper current\n" , upperCurrentWord);
    printf("%s upper guess\n" , upperGuess);

    //create letter frequency table
    int letterFrequency[26] = {0};
    for (int i = 0; upperCurrentWord[i]; i++) {
        letterFrequency[upperCurrentWord[i] - 'A']++;
    }

    char squares[MAX_WORD_LENGTH] = "";
    for (int i = 0; upperGuess[i]; i++) {
        char charGuess = upperGuess[i];
        char charWord = upperCurrentWord[i];
        if (charGuess == charWord) {
            strcat(squares, "G ");
            letterFrequency[charGuess - 'A']--;
        } else if (letterFrequency[charGuess - 'A'] > 0) {
            strcat(squares, "Y ");
            letterFrequency[charGuess - 'A']--;
        } else {
            strcat(squares, "R ");
        }
    }

    // Replace spaces with dashes
    char* token = strtok(squares, " ");
    strcpy(result, token);
    while ((token = strtok(NULL, " ")) != NULL) {
        strcat(result, "-");
        strcat(result, token);
    }

    return result;
}

int draw_game(){


    int spaceBetweenLetters = (xResolution / (MAX_WORD_LENGTH - 1)) - 30;
    for (int i = 0; i < MAX_WORD_LENGTH - 1; i++) {

        int index = word[i] - 'A';
        int xPos = (xResolution / 4) + (spaceBetweenLetters * i);

        drawSprite(letterSprites[index], xPos, 60);
    }
    return 0;
}
