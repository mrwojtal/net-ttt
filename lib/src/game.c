
#include "game.h"

void game_menu(void) {
    printf("\n=========Game Menu==========\n");
    printf("| 1. Display game history. |\n| 2. New game.             |\n| 3. Exit.                 |\n");
    printf("============================\n\n");
}

void game_instruction(void) {
    char example[FIELDS] = {'0', '1', '2',
                        '3', '4', '5',
                        '6', '7', '8'};
    printf("======================Game Instructions======================\n");
    printf("Welcome to network tic-tac-toe game!\n");
    printf("Play by typing indexes in range 0-8, according to game rules.\n");
    printf("Board indexing:\n");
    fields_print(example);
    printf("GLHF!\n\n");
    printf("=============================================================\n");

}

void fields_fill(char* fields) {
    for (int i = 0; i < FIELDS; i++) {
        fields[i] = '=';
    }
}

void fields_write(char* fields, char character) {
	int index = -1;
    for (;;) {
        printf("Provide index of a field to write %c:\n", character);
        printf("User input: ");
        if (scanf("%d", &index) != 1) {
            while (getchar() != '\n') {}
        }
	    if (index >= 0 && index <= 8) {
            if (fields[index] != '=') {
                printf("Given field is already occupied.\n");
            }
            else {
	            fields[index] = character;
                return;
            }
        }
        else {
            printf("Expected index in range 0-8.\n");
        }
    }
}

void fields_print(char* fields) {
    int i = 0;
	while(i < FIELDS) {
		printf("\n");
		for (int j = i; j < i+3; j++)
		{
			printf("| %c ", fields[j]);
		}
		printf("|\n");
        if (i < 6)
        {
		    printf("-------------");
        }
		i += 3;
	}
	printf("\n");
}

int game_summary(char* fields) {
	if (((fields[0] == 'X') && (fields[1] == 'X') && (fields[2] == 'X')) || /*horizontal win*/
		((fields[3] == 'X') && (fields[4] == 'X') && (fields[5] == 'X')) || /*horizontal win*/
		((fields[6] == 'X') && (fields[7] == 'X') && (fields[8] == 'X')) || /*horizontal win*/
		((fields[0] == 'X') && (fields[3] == 'X') && (fields[6] == 'X')) || /*vertical win*/
		((fields[1] == 'X') && (fields[4] == 'X') && (fields[7] == 'X')) || /*vertical win*/
		((fields[2] == 'X') && (fields[5] == 'X') && (fields[8] == 'X')) || /*vertical win*/
		((fields[0] == 'X') && (fields[4] == 'X') && (fields[8] == 'X')) || /*diagonal win*/
		((fields[6] == 'X') && (fields[4] == 'X') && (fields[2] == 'X')))   /*diagonal win*/ {
		return X_WINS; /*X wins*/
	}
	else if (((fields[0] == 'O') && (fields[1] == 'O') && (fields[2] == 'O')) || /*horizontal win*/
		     ((fields[3] == 'O') && (fields[4] == 'O') && (fields[5] == 'O')) || /*horizontal win*/
		     ((fields[6] == 'O') && (fields[7] == 'O') && (fields[8] == 'O')) || /*horizontal win*/
		     ((fields[0] == 'O') && (fields[3] == 'O') && (fields[6] == 'O')) || /*vertical win*/
		     ((fields[1] == 'O') && (fields[4] == 'O') && (fields[7] == 'O')) || /*vertical win*/
		     ((fields[2] == 'O') && (fields[5] == 'O') && (fields[8] == 'O')) || /*vertical win*/
		     ((fields[0] == 'O') && (fields[4] == 'O') && (fields[8] == 'O')) || /*diagonal win*/
		     ((fields[6] == 'O') && (fields[4] == 'O') && (fields[2] == 'O')))   /*diagonal win*/ {
		return O_WINS; /*O wins*/
	}
    else if (1 == check_fields(fields)) {
        return KEEP_PLAYING; /*keep playing*/
    }
	else {
		return DRAW; /*draw*/
	}

}

int check_fields(char* fields) {
    for (int i = 0; i < FIELDS; i++)
    {
        if (fields[i] == '=')
        {
            return 1;
        }
    }
    return 0;
}
