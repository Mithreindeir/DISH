#ifndef DSHELL_H
#define DSHELL_H

#include "ascii/draw.h"
#include "ascii/term.h"
#include "dio.h"
#include "dmatch.h"
#include "dconfig.h"
#include "dhash.h"

/* Public API*/

typedef struct dshell {
	struct text_buffer * buffer;

	struct dhash_t *table;
	struct termios original;

	char ** history;
	int num_history;

	struct dconfig *conf;
} dshell;

dshell *dshell_init();
void dshell_destroy(dshell * shell);

char *dshell_update(dshell *shell);
void dshell_loop(dshell * shell);
void dshell_loadconf(dshell *shell, const char * file);
void dshell_addfunc(dshell *shell, char *name, dhash_callback func, void *ctx);

/* Private Dshell Program */
void dshell_clear(dshell *shell);
void dshell_render(dshell *shell);

#endif
