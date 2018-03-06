#ifndef DMATCH_H
#define DMATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Basic Pattern Matching functionality.
 * Operators:
 * '*' matches anything
 * '?' matches one or zero characters of anything
 * '[abc]' matches a, b, or c
 * '[a-z]' matches lowercase a-z
 * '{a-z}' matches lowercase a-z or nothing
 * '$' matches any amount of the previous token
 *
 * Example:
 * '[A-z]$' matches any alphabetical string with no whitespace or other tokens.
 * */

int dmatch_text(const char *text, const char *match);
int dmatch_cl(const char *text, char **operators, int num_oper);
char **dmatch_lex(const char *match, int *num_oper);
int dmatch_expr(char *oper, char c);

#endif
