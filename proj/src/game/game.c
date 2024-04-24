#include <lcom/lcf.h>

#include "game.h"



char word[MAX_WORD];

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
            word[MAX_WORD - 1] = '\0'; // Add null terminator
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


void guess_word(char* guess, char ** result){

}
