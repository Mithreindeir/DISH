# DISH
The Dang Interactive Shell. 
C99 Library for creating psuedo-commandline programs. 

### Features
* Fast and Simple
* Command history
* Config files for making custom syntax highlighting
* Easy API using callback functions.
* Customizable prompt
* Pattern matching


Example program
````
#include <stdio.h>
#include <stdlib.h>
#include "dshell.h"

int example(int argc, char ** argv, void *data)
{
	for (int i = 0; i < argc; i++) {
		writef("\r\n%s", argv[i]);
	}
	write_str(data);
	return 0;
}

int main()
{
	dshell *shell = dshell_init();
	dshell_loadconf(shell, "color.example");
	dshell_addfunc(shell, "test", &example, "\r\nHello world!!");
	char buf[256];
	memset(buf, 0, 256);
	char * prompt = &buf[0];
	shell->conf->prompt = &prompt;
	char * str = NULL;
	int iter = 0;
	do {
		snprintf(prompt, 256, "[%#x]->", iter++);
		str = dshell_update(shell);
	} while (!str || (str && !!strcmp(str, "quit")));

	dshell_destroy(shell);
	return 0;
}
````
