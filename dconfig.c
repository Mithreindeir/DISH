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
			//getchar();
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

	int len = dmatch_text(string, "*{\t }$={\t }$[A-z]$\n");
	if (len == 0)
		return *blen=-1, p;
	*blen += len;
	len--;
	//Retrieve matching expression
	int ml = dmatch_text(string, "*{\t }$=");
	int assign = ml;
	if (!ml) return *blen=-1, p;
	ml-=2;
	while ((ml--, string[ml]=='\t' || string[ml] == ' '));
	ml++;
	char * expr = malloc(ml+1);
	strncpy(expr, string, ml);
	expr[ml] = 0;

	assign++;
	while (string[assign]=='\t'||string[assign]==' ') assign--;
	int clen = len - assign;
	char * color = malloc(clen+1);
	strncpy(color, string+assign, clen);
	color[clen] = 0;

	free(color);
	p.keyword = expr;
	return p;
}
