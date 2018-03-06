#ifndef DIO_H
#define DIO_H

#include <stdio.h>
#include <stdlib.h>
#include "ascii/term.h"

/*Dish I/O File*/

char * dish_readline(char ** history, int num_history);
int key_process(int * arrowkey);

char * dish_split(const char * line);
char ** dish_splitline(const char * input, int * num_param);

#endif
