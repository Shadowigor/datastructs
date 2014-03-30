
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
	ds_value_t v;
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
	int len_sep;
	ds_value_t value;
	ds_list_t nodes;
} *ds_trie_t;

ds_string_t ds_string_new(char *str);
int ds_string_append(ds_string_t dest, ds_string_t to_append);
int ds_string_copy(ds_string_t dest, ds_string_t src);
ds_list_t ds_string_split(ds_string_t string, char delim);
int ds_string_cut(ds_string_t string, int left, int right);
int ds_string_insert(ds_string_t string, int position);
ds_string_t ds_string_extract(ds_string_t string, int start, int end);
int ds_string_cutout(ds_string_t string, int start, int end);
int ds_string_destroy(ds_string_t string);

ds_array_t ds_array_new(int len, size_t size);
int ds_array_add_value(ds_array_t array, ds_value_t value);
int ds_array_add_array(ds_array_t array, ds_array_t value);
int ds_array_insert_value(ds_array_t array, ds_value_t value, int position);
int ds_array_insert_array(ds_array_t array, ds_array_t value, int position);
int ds_array_remove_value(ds_array_t array, int position);
int ds_array_remove_array(ds_array_t, array, int position, int len);
ds_value_t ds_array_search(ds_array_t array, ds_value_t value, ds_value_t mask);
int ds_array_destroy(ds_array_t array);

ds_list_t ds_list_new(void);
int ds_list_add(ds_value_t value);
int ds_list_remove(ds_list_t node);
ds_list_t ds_list_search(ds_list_t list, ds_value_t value, ds_value_t mask);
int ds_list_destroy(ds_list_t list);

ds_trie_t ds_trie_new(char seperator, int len);
int ds_trie_add(ds_value_t key, ds_value_t value);
int ds_trie_remove(ds_value_t key);
ds_value_t ds_trie_search(ds_value_t key);
int ds_trie_destroy(ds_trie_t trie);

#endif /* DATASTRUCTS_H_ */
