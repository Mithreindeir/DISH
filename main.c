#include <stdio.h>
#include <stdlib.h>
#include "dshell.h"

char * example_text =
"//This is a comment\r\n"
"int example(struct text_buffer *text, int argc, const char ** argv, void *data)\r\n"
"	for (int i = 0; i < argc; i++) {\r\n"
"		text_buffer_print(text, \"\\r\\n%s\", argv[i]);\r\n"
"	}\r\n"
"	text_buffer_print(text, \"\\r\\n%s\", data);\r\n"
" 	return 0;\r\n"
"}\r\n";

int example(struct text_buffer *text, int argc, char ** argv, void *data)
{
	for (int i = 1; i < argc; i++) {
		text_buffer_print(text, "\r\n%s", argv[i]);
	}
	if (argc>=3) {
		char const**save=NULL;
		int slen=0;
		int clen = dmatch_text(argv[2], argv[1],&save,&slen);
		writef("%d %d\r\n", clen, slen);
		for (int i = 0; i < slen; i++) {
			writef("s:%s %p\r\n", save[i], save[i]);
		}
	}
	text_buffer_print(text, "\r\n%s", data);
	return 0;
}

int main()
{
	dshell *shell = dshell_init();
	dshell_loadconf(shell, "color.example");
	dshell_addfunc(shell, "test", &example, example_text);
	char buf[256];
	memset(buf, 0, 256);
	char * prompt = &buf[0];
	//shell->conf->prompt = &prompt;
	char * str = NULL;
	int iter = 0;
	do {
		snprintf(prompt, 256, "[%#x]->", iter++);
		str = dshell_update(shell);
		dshell_render(shell);
	} while (!str || (str && !!strcmp(str, "quit")));

	dshell_destroy(shell);
	return 0;
}
