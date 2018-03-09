#include "dshell.h"

dshell *dshell_init()
{
	dshell *shell = malloc(sizeof(dshell));

	shell->original = get_term();
	shell->buffer = text_buffer_init();
	shell->table = dhash_table_init(50);
	shell->conf = NULL;
	shell->history = NULL;
	shell->num_history = 0;
	set_term(raw_term());

	return shell;
}

void dshell_destroy(dshell *shell)
{
	if (!shell) return;

	for (int i = 0; i < shell->num_history; i++)
		free(shell->history[i]);
	free(shell->history);
	dhash_table_destroy(shell->table);
	text_buffer_destroy(shell->buffer);
	if (shell->conf)
		dconfig_destroy(shell->conf);
	set_term(shell->original);
	free(shell);
}

void dshell_loadconf(dshell *shell, const char *file)
{
	if (shell->conf)
		dconfig_destroy(shell->conf);
	shell->conf = dconfig_load(file);
}

char *dshell_update(dshell *shell)
{
	if (shell->conf && shell->conf->prompt) {
		writef("\r\n%s", *shell->conf->prompt);
	} else {
		write_str("\r\n>");
	}
	char * line = dish_readline(shell->history, shell->num_history);
	if (line) {
		shell->num_history++;
		if (!shell->history) {
			shell->history = malloc(sizeof(char*));
		} else {
			shell->history = realloc(shell->history, sizeof(char*)*shell->num_history);
		}
		shell->history[shell->num_history-1] = line;
	}
	write_str("\r\n");
	int num_arg = 0;
	char ** args = dish_splitline(line, &num_arg);
	if (!num_arg) return line;
	struct dhash_entry *cmd = dhash_table_lookup(shell->table, args[0]);
	if (!cmd) {
		writef("%s not found\r\n", args[0]);
	} else {
		if (cmd->callback){
			cmd->callback(shell->buffer, num_arg, args, cmd->context);
		}
	}
	for (int i = 0; i < num_arg; i++)
		free(args[i]);
	free(args);
	return line;
}

void dshell_addfunc(dshell *shell, char *name, dhash_callback func, void *ctx)
{
	struct dhash_entry *e = dhash_entry_init(name, func, ctx);
	dhash_table_insert(shell->table, e);
}

void dshell_clear(dshell *shell)
{
	text_buffer_clear(shell->buffer);
}

void dshell_render(dshell *shell)
{
	shell->buffer->pairs = shell->conf->pairs;
	shell->buffer->num_pairs = shell->conf->num_pairs;

	text_buffer_render(shell->buffer);
	text_buffer_clear(shell->buffer);
	writef("\x1b[%d;0m", 0);
}
