#include "dhash.h"

struct dhash_t *dhash_table_init(int num_buckets)
{
	struct dhash_t *table = malloc(sizeof(struct dhash_t));

	table->num_buckets = num_buckets;
	table->buckets = calloc(num_buckets, sizeof(struct dhash_entry*));

	return table;
}

void dhash_table_destroy(struct dhash_t *table)
{
	if (!table) return;

	for (int i = 0; i < table->num_buckets; i++) {
		struct dhash_entry *head = table->buckets[i];
		struct dhash_entry *tmp = NULL;
		while (head) {
			tmp = head->next;
			free(head);
			head = tmp;
		}
	}

	free(table->buckets);
	free(table);
}

void dhash_table_insert(struct dhash_t *table, struct dhash_entry *entry)
{
	int idx = entry->hash % table->num_buckets;
	dhash_entry_add(&table->buckets[idx], entry);
}

struct dhash_entry *dhash_table_lookup(struct dhash_t *table, const char *name)
{
	unsigned long hash = hash_name(name);
	int idx = hash % table->num_buckets;
	return dhash_entry_find(table->buckets[idx], name);
}

void dhash_table_resize(struct dhash_t *table, int new_size)
{
	struct dhash_entry **old = table->buckets;
	int old_size = table->num_buckets;

	table->buckets = calloc(sizeof(struct dhash_entry*), new_size);
	table->num_buckets = new_size;

	for (int i = 0; i < old_size; i++ ) {
		struct dhash_entry *head = old[i];
		struct dhash_entry *next = NULL;
		while (head) {
			next = head->next;
			dhash_table_insert(table, head);
			head = next;
		}
	}

	free(old);
}

unsigned long hash_name(const char *name)
{
	unsigned long hash = 5381;
	char c;
	while ((c=*name++))
		hash = ((hash << 5) + hash) + (unsigned char)c;
	return hash;
}

void dhash_entry_add(struct dhash_entry **head, struct dhash_entry *e)
{
	if (!*head) {
		*head = e;
		return;
	}
	struct dhash_entry *cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = e;
}

struct dhash_entry *dhash_entry_init(char *name, dhash_callback func, void *ctx)
{
	struct dhash_entry *entry = malloc(sizeof(struct dhash_entry));

	entry->callback = func;
	entry->context = ctx;
	entry->name = name;
	entry->hash = hash_name(name);
	entry->next = NULL;

	return entry;
}

struct dhash_entry *dhash_entry_find(struct dhash_entry *head, const char *name)
{
	while (head && !!strcmp(head->name, name)) head=head->next;
	return head;
}
