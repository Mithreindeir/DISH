#ifndef DCMD_H
#define DCMD_H

/* Dish Builtin commands */
#include "ascii/draw.h"
typedef int(*)(struct text_buffer *buf, void *dat, char **argv, int argc) cmd_func;

struct dcmd {
	char * usage;
	int(*execute)(struct text_buffer * buf, void * user_data,const char ** argv, int argc);
	cmd_func execute;
};

#endif
