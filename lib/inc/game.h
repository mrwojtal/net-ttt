#ifndef GAME_H
#define GAME_H

#include <stdio.h>

#define FIELDS 9

#define KEEP_PLAYING 0
#define X_WINS 1
#define O_WINS 2
#define DRAW 3

void game_menu(void);

void game_instruction(void);

void fields_fill(char* fields);

void fields_write(char* fields, char character);

void fields_print(char* fields);

int game_summary(char* fields);

int check_fields(char* fields);

#endif /* GAME_H */