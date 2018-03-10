#include "dconfig.h"

struct dconfig *dconfig_load(const char *file)
{
	struct dconfig *conf = malloc(sizeof(struct dconfig));
	conf->pairs = NULL;
	conf->num_pairs = 0;
	conf->prompt = NULL;
	conf->verbose = 0;

	FILE * f = fopen(file, "r");
	char *buf = NULL;
	long size = 0;
	if (f) {
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		buf = malloc(size+1);
		memset(buf, 0, size+1);
		rewind(f);
		fread(buf, 1, size, f);
		fclose(f);
	}
	int iter = 0;;
	if (buf) {
		while (iter < size) {
			struct color_pair cp = parser_color(buf+iter, &iter);
			if (iter == -1) break;
			conf->pairs = color_pair_add(conf->pairs,&conf->num_pairs, cp);
		}
		free(buf);
	}
	return conf;
}

void dconfig_destroy(struct dconfig *conf)
{
	if (!conf) return;
	for (int i = 0; i < conf->num_pairs; i++) {
		free(conf->pairs[i].keyword);
	}
	free(conf->pairs);
	free(conf);
}

void dconfig_setprompt(struct dconfig *conf, char ** prompt)
{
	conf->prompt = prompt;
}

void dconfig_verbose(struct dconfig *conf)
{
	conf->verbose = 1;
}

struct color_pair parser_color(const char * string, int * blen)
{
	struct color_pair p;
	p.keyword = NULL;
	p.len = 0;
	p.color = 0;

	char const**saved = NULL;
	int slen = 0;
	int len = dmatch_text(string, "*^{\t }$={\t }$^[A-z]$",&saved,&slen)+1;
	if (len == 1)
		return *blen=-1, p;
	*blen += len;
	len--;
	//Retrieve matching expression
	int ml = saved[0]-string;
	int assign = saved[1]-string;
	free(saved);
	if (!ml) return *blen=-1, p;
	if (ml<=0)
		return *blen=-1, p;
	char * expr = malloc(ml+1);
	strncpy(expr, string, ml);
	for (int i = 0; i < ml; i++) {
		if ((i+1) < ml && expr[i]=='\\') {
			char rc = expr[i];
			switch (expr[i+1]) {
				case 'n':rc='\n';break;
				case 'r':rc='\r';break;
				case 't':rc='\t';break;
			}
			if (rc != expr[i]) {
				expr[i] = rc;
				memmove(expr+i+1, expr+i+2, ml-(i+2));
				ml--;
			}
		}
	}
	expr[ml] = 0;
	int clen = len - assign;
	char * color = malloc(clen+1);
	strncpy(color, string+assign, clen);
	color[clen] = 0;
	p.color = 37;

	if (!strcmp(color, "black")) {
		p.color = 30;
	} else if (!strcmp(color, "red")) {
		p.color = 31;
	} else if (!strcmp(color, "green")) {
		p.color = 32;
	} else if (!strcmp(color, "yellow")) {
		p.color = 33;
	} else if (!strcmp(color, "blue")) {
		p.color = 34;
	} else if (!strcmp(color, "magenta")) {
		p.color = 35;
	} else if (!strcmp(color, "yellow")) {
		p.color = 33;
	}

	free(color);
	p.keyword = expr;
	return p;
}
