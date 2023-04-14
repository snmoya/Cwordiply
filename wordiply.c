#include <stdio.h>
#include <csse2310a1.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//This function is to copy a string to another
void string_copy(char **original, const char *new){
    *original = malloc(strlen(new) + 1);
    strcpy(*original, new);
}

//This function change a string to Upper case
void string_to_upper(char *original){
    for (int i = 0; original[i] != '\0'; i++) {
        original[i] = toupper(original[i]);
    }	
}

/* --start argument checker.
 * if specified, this option argument is followed by a 3 or 4 letter
 *  starter word.
 * If the argument is not supplied, then the program must choose a random word.
 */
int start_check(int argc, char *argv[], char **x){
    int flag = 1;
    int argCount = 0;

    for (int i = 0; i < argc; i++) {
        //Check if there is a --start argument
        if (strcmp(argv[i], "--start") == 0) {
            argCount += 1;
            //if the argument is given without value
            if (i == (argc - 1) || strncmp(argv[i], argv[i + 1], 2) == 0) {
                flag = 0;
                break;
            //If a value is given
            } else {
		*x = malloc(strlen(argv[i + 1]) + 1);
  		strcpy(*x, argv[i + 1]);
            }
	}
    }
    //if the argument is more than once
    if (argCount > 1) {
        flag = 0;
    }
    //If the argument is not given
    if (argCount == 0) {
        string_copy(x, get_wordiply_starter_word(0));
    }
    return flag;
}

/*if specified, this option argument is followed by the number 3 or 4.
 * if argument is not given, then the random starter word can be
 * either 3 or 4 letters long
*/
int len_check(int argc, char *argv[], char **x){
    int flag = 1;
    int argCount = 0;
    int argValue = 0;

    for (int i = 0; i < argc; i++) {
        //Check if there is a --len argument
        if (strcmp(argv[i], "--len") == 0) {
            argCount += 1;
            //if the argument is given without value
            if (i == (argc - 1) || strncmp(argv[i], argv[i + 1], 2) == 0) {
                flag = 0;
                break;
            //if is given with value.
            } else if (strcmp(argv[i], "--len") == 0) {
                if (strcmp(argv[i + 1], "3") == 0 ||
                        strcmp(argv[i + 1], "4") == 0) {
                    argValue = atoi(argv[i + 1]);
		    string_copy(x, get_wordiply_starter_word(argValue));
		} else {
		    flag = 0;
		    break;
		}	
            }
        }
    }
    //if the argument is more than once
    if (argCount > 1) {
        flag = 0;
    }
    return flag;
}

/*if specified, this option argument is followed by the name of a file.
 * If a dictionary filename is not specified, the default should be used.
 */
int dict_check(int argc, char *argv[], char **x){
    int flag = 1;
    int argCount = 0;
    char defaultDict[] = "/usr/share/dict/words";

    for (int i = 0; i < argc; i++) {
        //Check if there is a --dictionary argument
        if (strcmp(argv[i], "--dictionary") == 0) {
            argCount += 1;
            //if the argument is given without value
            if (i == (argc - 1) || 
	            strncmp(argv[i], argv[i + 1], 2) == 0) {
                flag = 0;
                break;
            //if is given with value.
            } else {
	        *x = malloc(strlen(argv[i + 1]) + 1);
                strcpy(*x, argv[i + 1]);
            }
        }
    }
    //if the argument is more than once
    if (argCount > 1) { 
        flag = 0;
    }
    //If the argument is not given
    if (argCount == 0) {
        *x = malloc(strlen(defaultDict + 1));
        strcpy(*x, defaultDict);
    }
    return flag;
}

/*Checks if --start and --len arguments are specified.
 */
int both_args_check(int argc, char *argv[]){
    int startFlag = 0;
    int lenFlag = 0;
    int flag = 1;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--start") == 0) {
            startFlag = 1;
	} else if (strcmp(argv[i], "--len") == 0) {
	    lenFlag = 1;
	}	
    }
    if (startFlag > 0 && lenFlag > 0) {
        flag = 0;
    }
    return flag;
}

/*Check any other type of argument
 */
int other_args(int argc, char *argv[]){
    int flag = 1;
	
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--dictionary") == 0 || 
                    strcmp(argv[i], "--start") == 0 ||
	       	    strcmp(argv[i], "--len") == 0) {
	        i++;
	    } else {
	        flag = 0;
	        break;
	    }
        }
    }
    return flag;
}

/*This function check the start Word
 * Check if a non alpha character in it and if the len is between 3 and 4.
 */
int start_word_check(char *x){
    int flag = 1;
	
    for (int i = 0; x[i] != '\0'; i++) {
        if (isalpha(x[i]) == 0) {
            flag = 0;
	}
    }
    if (strlen(x) < 3 || strlen(x) > 4) {
        flag = 0;
    }
    return flag;
}

/*Function that check user Guess
 * Check if the guess contain only letter
 * if the guess is the same as the starter word
 * if contain the starter word
 * if the guess is in the dictionary
 * and if the guess was already wrote.
 */
int check_user_guess(char *guess, char *startWord, char **arrayWords, 
        char **arrayGuesses, int guessCount, int arraySize){
    int flag = 0;
    int guessPresent = 0;

    string_to_upper(guess);
    //Check if the guess only contains letters
    for (int i = 0; i < (strlen(guess)); i++) {
        if (isalpha(guess[i]) == 0) { 
	    printf("Guesses must contain only letters - try again.\n");
	    flag += 1;
	    break;
	}
    }
    //Check if the start word is equal to the user guess
    if (strcmp(guess, startWord) == 0) {
        printf("Guesses can't be the starter word - try again.\n");
	flag += 1;
    //Check if the start word is in the guess
    } else if (strstr(guess, startWord) == NULL && flag == 0) {
	printf("Guesses must contain the starter word - try again.\n");
	flag += 1;
    }
    //Check if word is in the dictionary
    for (int i = 0; i < (arraySize - 2); i++) {
	if (strcmp(guess, arrayWords[i]) == 0) {
	    guessPresent += 1;
	}
    }
    if (guessPresent == 0 && flag == 0) {
        printf("Guess not found in dictionary - try again.\n");
        flag += 1;
    }
    //Check if the guess was already wrote.
    for (int i = 0; i < (guessCount - 1); i++) {
        if (guessCount > 1 && strcmp(guess, arrayGuesses[i]) == 0) {
	    printf("You've already guessed that word - try again.\n");
            flag += 1;
	    break;
	}
    }
    return flag;
}

/*This function print the longest word in the dictionary that match
 * the start word and print the respective lenght.
 */
void longest_print(char **array, int arraySize){
    int longest = 0;
    char **longestWords = malloc(sizeof(char*) * 1);
    int count = 0;
    int arraylen = 2;
    int firstAlphaFlag = 0;
    int secondAlphaFlag = 0;
    
    for (int i = 0; i < (arraySize - 2); i++) {
        int len = strlen(array[i]);
	for (int j = 0; j < strlen(array[i]); j++) {
            if (isalpha(array[i][j]) == 0) {
                firstAlphaFlag = 1;
	    }
	}    
	if (len > longest && firstAlphaFlag == 0) {
            longest = len;
        }
	firstAlphaFlag = 0;
    }

    for (int i = 0; i < (arraySize - 2); i++) {
        for (int j = 0; j < strlen(array[i]); j++) {
            if (isalpha(array[i][j]) == 0) {
                secondAlphaFlag = 1;
            }
        }
    	
        if (strlen(array[i]) == longest && secondAlphaFlag == 0) {
            longestWords = realloc(longestWords, sizeof(char*) * arraylen);
            longestWords[arraylen - 2] = malloc(sizeof(char*) *\
                    (strlen(array[i]) + 1));
            strcpy(longestWords[arraylen - 2], array[i]);
            arraylen += 1;
            count += 1;
        }
        secondAlphaFlag = 0;
    }
    //printf("%i\n", longest);  
    for (int i = 0; i < count; i++) {
        printf("%s (%ld)\n", longestWords[i], strlen(longestWords[i]));
    }	
}

/*This function print the longest guess at the end of the game,
 *with the value.
 */
void longest_guess(char **array, int arraySize){
    int longest = 0;
    char **longestWords = malloc(sizeof(char*) * 1);
    int count = 0;
    int arraylen = 2;

    for (int i = 0; i < (arraySize - 1); i++) {
        int len = strlen(array[i]);
        if (len > longest) {
            longest = len;
	}
    }

    for (int i = 0; i < (arraySize - 1); i++) {
        if (strlen(array[i]) == longest) {
	    longestWords = realloc(longestWords, sizeof(char*) * arraylen);
	    longestWords[arraylen - 2] = malloc(sizeof(char*) *\
                    (strlen(array[i]) + 1));
	    strcpy(longestWords[arraylen - 2], array[i]);	
	    arraylen += 1;
	    count += 1;
	}
    }

    for (int i = 0; i < count; i++) {
	printf("%s (%ld)\n", longestWords[i], strlen(longestWords[i]));
    }

}

/* Function to get the total sum of lenght of all guesses
 */
int total_sum(char **array, int guessCount){
    int totalSum = 0;

    for (int i = 0; i < (guessCount - 1); i++) {
        int len = strlen(array[i]);
	totalSum += len;
    }
    return totalSum;
}

/*In this function is where the logic is places to run the program.
 */
int main(int argc, char *argv[]){
    char *startWord; 
    char *dictPath;
    char *userGuess;
    userGuess = (char *)malloc(sizeof(char) * 20);
    int guessCount = 1;
    char **validGuesses;
    validGuesses = malloc(sizeof(char*) * 5); //max num of guesses is 5.
    int arraySize = 2;
    char **matchingWords = malloc(sizeof(char*) * 1);
    const unsigned maxLength = 50;
    char buffer[maxLength];

    //Grouping all the checks functions.
    if (start_check(argc, argv, &startWord) == 0 || 
            len_check(argc, argv, &startWord) == 0 ||
            dict_check(argc, argv, &dictPath) == 0 ||
            both_args_check(argc, argv) == 0 || other_args(argc, argv) == 0) {
        fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
		" --len length] [--dictionary filename]\n");
        exit(1);
    }
    if (start_word_check(startWord) == 0) {
        fprintf(stderr, "uqwordiply: invalid starter word\n");
        exit(2);
    } else {
	string_to_upper(startWord);
    }

    FILE *in = fopen(dictPath, "r");
    if (in == NULL) {
        fprintf(stderr, "uqwordiply: dictionary file \"%s\" cannot"
		" be opened\n", dictPath);
	exit(3);
    } else {
	//Read the dictionary
	while (fgets(buffer, maxLength, in)) {
            string_to_upper(buffer);
            if (strstr(buffer, startWord) != 0) {
                buffer[strlen(buffer) - 1] = '\0';
	        matchingWords = realloc(matchingWords, sizeof(char*) *\
                        arraySize); 
                matchingWords[arraySize - 2] = malloc(sizeof(char*) *\
			(strlen(buffer) + 1));
		strcpy(matchingWords[arraySize - 2], buffer);
		arraySize += 1;
	    }
        }	
    }
    //Initialize the game.
    printf("Welcome to UQWordiply!\n");
    printf("The starter word is: %s\n", startWord);
    printf("Enter words containing this word.\n");

    //Prompt the user to play until the end of the game.
    while (guessCount < 6) {
        if (guessCount < (arraySize - 1)) {
	    fprintf(stdout, "Enter guess %i:\n", guessCount); 
	    fgets(userGuess, maxLength, stdin);
	    string_to_upper(userGuess);
	    userGuess[strcspn(userGuess, "\n")] = 0;
	    if (!feof(stdin)) {
	        if (check_user_guess(userGuess, startWord,
		        matchingWords, validGuesses, guessCount, 
		        arraySize) == 0) {
		    validGuesses[guessCount - 1] = malloc(sizeof(char) *\
                            strlen(userGuess) + 1);
		    strcpy(validGuesses[guessCount - 1], userGuess);
		    guessCount += 1;
		}	
	    } else if (feof(stdin) && guessCount > 1) {
                break;
	    } else {
                exit(4);
	    }
	} else {
	    break;
	}
    }

    printf("\n");
    printf("Total length of words found: %i\n",
            total_sum(validGuesses, guessCount));
    printf("Longest word(s) found:\n");
    longest_guess(validGuesses, guessCount); 
    printf("Longest word(s) possible:\n");
    longest_print(matchingWords, arraySize);
    fclose(in);
    exit(0);
}
