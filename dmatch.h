#ifndef DMATCH_H
#define DMATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ascii/term.h"

#define MAX_OPER_LEN 16
#define MAX_OPER 32

/* Basic Pattern Matching functionality.
 * Operators:
 * '*' matches anything
 * '?' matches one or zero characters of anything
 * '[abc]' matches a, b, or c
 * '[a-z]' matches lowercase a-z
 * '{a-z}' matches lowercase a-z or nothing
 * '$' matches any amount of the previous token
 * '^' saves the current length of the string
 *
 * Example:
 * '[A-z]$' matches any alphabetical string with no whitespace or other tokens.
 * */

int dmatch_text(const char *text, const char *match, char const***save,int *slen);
int dmatch_cl(const char *text, char operators[MAX_OPER][MAX_OPER_LEN],int num_oper,char const***save,int*slen);
void dmatch_lex(const char *match, char operators[MAX_OPER][MAX_OPER_LEN], int *num_oper);
int dmatch_expr(char *oper, char c);

#endif
