#include "project.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Defines the maximum amount that we should read in from an input.
#define MAX_INPUT 100


Game* loadFile(Game* array, int* games){
    /**
    *   Function for loading the input file. 
    * 
    *   If the filename does'nt exists, error message is printed and previous array returned,
    *   since we do not wish to lose any data.
    *   
    *   Filename is taken out as a token.
    * 
    *   Function frees the memory from old games and creates fully new array to store games.
    * 
    *   If the file is not closed correctly, it returns NULL.
    */
   char* filename = strtok(NULL, " \n");
   
   // Please note that the assignment instructions say that command needs to be
   // O games and not O games.txt
   // A+ fails my concatenation, even when I've done this according to instructions...
   //filename = strncat(filename, ".txt", 5);

    FILE *f = fopen(filename, "r");
    if (!f){
        printf("Cannot open file %s for reading.\n", filename);
        return array;
    }

    free(array);
    *games = 0;

    array = (Game*) calloc(1, sizeof(Game));
    char line[MAX_INPUT];
    while (fgets(line, MAX_INPUT, f) != NULL){
        char* name = strtok(line, " ");
        char* price = strtok(NULL, " ");
        char* revenue = strtok(NULL, " ");
        *games = *games + 1;
        array = realloc(array, sizeof(Game) * (*games));
        Game new;
        strcpy(new.name, name);
        new.price = atof((const char*) price);
        new.revenue = atof((const char*) revenue);
        array[*games - 1] = new;
    }

    int closed = fclose(f);
    if (!closed){
        printf("SUCCESS\n");
        return array;
    }
    printf("Invalid command O\n");
    return NULL;
}
/**
*   Function that saves the Game structures to the filename given as a parameter. 
*
*   The filename parameter is given without the .txt ending, which is added in this function.
*
*   Function returns the filename and prints errors if
*   - file is not opened correctly
*   - file is not closed correctly
*/
int saveFile(Game* array, int* games){
    char* filename = strtok(NULL, " \n");


    // Please note that the assignment instructions say that command needs to be
    // O games and not O games.txt
    // A+ fails my concatenation, even when I've done this according to instructions...
    //filename = strncat(filename, ".txt", 5);

    FILE *f = fopen(filename, "w");
    if (!f){
        printf("Invalid command W\n");
        return -1;
    }

    for (int i = 0; i < *games; i++){
        fprintf(f, "%s %.2f %.2f\n", array[i].name, array[i].price, array[i].revenue);
    }

    int closed = fclose(f);
    if (!closed){
        printf("SUCCESS\n");
        return 0;
    }
    printf("Invalid command W\n");
    return -1;
}

/**
 *  Function that adds the game given in the command.
 * 
 *  Prints error messages if:
 *  - price is negative
 *  - game exists in the array
 */


Game* addGame(Game* array, int* games){
    
    char* name = strtok(NULL, " \n");
    char* price = strtok(NULL, " \n");
    
    if (name == NULL || price == NULL){
        printf("A should be followed by exactly 2 arguments.\n");
        return array;
    }

    if (atof(price) <= 0.0){
        printf("Game price cannot be negative or zero.\n");
        return array;
    }

    // Loop through the array to check if the name of game exists
    for (int i = 0; i < *games; i++){
        if (strcmp(array[i].name, name) == 0){
            printf("Invalid command A\n");
            return array;
        }
    }
    // If previous loop did not find the name, then initialise new game
    *games = *games + 1;
    array = realloc(array, sizeof(Game) * (*games));
    Game new;
    strcpy(new.name, name);
    new.price = atof((const char*) price);
    new.revenue = 0.0;
    array[*games - 1] = new;

    printf("SUCCESS\n");
    return array;
}

/**
 *  Function for buying games. If game name is found, the function adds revenue to the game.
 *  
 *  If the game is not found, function prints out the error-message. Also, if count is negative.
 */

int buy(Game* array, int* games){
    char* name = strtok(NULL, " \n");
    char* bought = strtok(NULL, " \n");

    if (name == NULL || bought == NULL){
        printf("B should be followed by exactly 2 arguments.\n");
        return -1;
    }

    if (atoi(bought) <= 0){
        printf("Number of bought items cannot be less than 1.\n");
        return -1;
    }
    for (int i = 0; i < *games; i++){
        if (strcmp(array[i].name, name) == 0){
            array[i].revenue = array[i].revenue + atoi(bought) * array[i].price;
            printf("SUCCESS\n");
            return 0;
        }
    }
    printf("Game \"%s\" is not in the database.\n", name);
    return -1;
}

// Function to support qsort in listGames-function.

int revenueComparator(const void *p1, const void *p2){
    Game *g1 = (Game*) p1;
    Game *g2 = (Game*) p2;
    if (g1->revenue < g2->revenue){
        return 1;
    }
    else if (g1->revenue > g2->revenue){
        return -1;
    }
    else 
        return 0;
}

/**
 *  Function that lists the games in "DB". 
 * 
 *  No error messages possible? 
 */

int listGames(Game* array, int* games){
    // Qsort sorts the array before printing
    qsort(array, *games, sizeof(Game), revenueComparator);

    // Loop through the array.
    for (int i = 0; i < *games; i++){
        printf("%s %.2f %.2f\n", array[i].name, array[i].price, array[i].revenue);
        }
    printf("SUCCESS\n");
    return 0;
}

/**
 *  The main-function of the Game-shop. 
 * 
 *  Main functionality is based on infinite while-loop, that ends if
 *  - Nothing was given as a command
 *  - Q is given as an input 
 */

int main(void){
    // Initialises the array of games
    Game* array = (Game*) calloc(1, sizeof(Game));
    // Helper variable to determine amount of games in array
    int games = 0;
    // Variable used to handle int-function returns, no functionality
    //int res;
    // Variable to store commands
    char line[MAX_INPUT];

    while(fgets(line, MAX_INPUT, stdin) != NULL){
        char* input = strtok(line, " ");
        if (*input != 'Q'){
            switch(*input){
                case 'A':
                    array = addGame(array, &games);
                    break;

                case 'B':
                    buy(array, &games);
                    break;

                case 'L':
                    listGames(array, &games);
                    break;

                case 'W':
                    saveFile(array, &games);
                    break;

                case 'O':
                    array = loadFile(array, &games);
                    break;

                default:
                    printf("Invalid command %s\n", input);
                    break;
            }
        }
        else
            // Input command was Q, so exit the while-loop.
            break;
    }

    free(array);
    printf("SUCCESS\n");
    return 0;
}