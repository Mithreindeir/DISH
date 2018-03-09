#ifndef DHASH_H
#define DHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ascii/term.h"
#include "ascii/draw.h"

typedef
int(*dhash_callback)(struct text_buffer *text,int argc,char **argv,void *ctx);

/*Dish hash table for storing commands*/
struct dhash_entry {
	char * name;
	unsigned long hash;
	struct dhash_entry * next;
	dhash_callback callback;
	void * context;
};

struct dhash_t {
	struct dhash_entry ** buckets;
	int num_buckets;
};


/*Hash Table functions*/
struct dhash_t *dhash_table_init(int num_buckets);
void dhash_table_destroy(struct dhash_t *table);

void dhash_table_insert(struct dhash_t *table, struct dhash_entry *entry);
void dhash_table_remove(struct dhash_t *table, struct dhash_entry *entry);
void dhash_table_resize(struct dhash_t *table, int new_size);
struct dhash_entry *dhash_table_lookup(struct dhash_t *table, const char *name);

/*Helper Functions*/
unsigned long hash_name(const char *name);
void dhash_entry_add(struct dhash_entry **head, struct dhash_entry *e);
struct dhash_entry *dhash_entry_find(struct dhash_entry *head, const char *name);
struct dhash_entry *dhash_entry_init(char *name, dhash_callback func, void *ctx);

#endif
