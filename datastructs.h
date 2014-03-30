
#ifndef DATASTRUCTS_H_
#define DATASTRUCTS_H_

#include <stdlib.h>

typedef void* ds_value_t;

typedef struct ds_string_s
{
	unsigned int len;
	char *start;
	char *end;
} *ds_string_t;

typedef struct ds_array_s
{
	unsigned int len;
	size_t size;
	ds_value_t value;
} ds_array_t;

typedef struct ds_list_s
{
	size_t size;
	ds_value_t value;
	struct ds_list_s *prev;
	struct ds_list_s *next;
} *ds_list_t;

typedef struct ds_trie_s
{
	ds_value_t value;
	ds_list_t nodes;
} *ds_trie_t;

ds_string_t ds_string_new(char *str);
int ds_string_append(ds_string_t dest, ds_string_t to_append);
int ds_string_copy(ds_string_t dest, ds_string_t src);
ds_list_t ds_string_split(ds_string_t string, char delim);

ds_list_t ds_list_new(void);
int ds_list_add(ds_value_t value);
int ds_list_remove(ds_list_t node);
ds_list_t ds_list_search(ds_list_t list, ds_value_t value, ds_value_t mask);

ds_trie_t ds_trie_new(void);
int ds_trie_add();

#endif /* DATASTRUCTS_H_ */
