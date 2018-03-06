#ifndef DCONFIG_H
#define DCONFIG_H

/*Config file importing
 * And runtime config API
 * */
#include "ascii/syntax.h"
#include "dmatch.h"

struct dconfig {
	struct color_pair *pairs;
	int num_pairs;
	char ** prompt;
	int verbose;
};

struct dconfig *dconfig_load(const char * filename);
void dconfig_destroy(struct dconfig *conf);
/* Runtime Config API */
void dconfig_setprompt(struct dconfig *conf, char ** prompt);
void dconfig_verbose(struct dconfig *conf);

/*Parsing Helper Functions*/
struct color_pair parser_color(const char * string, int * blen);

#endif
