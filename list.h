#ifndef __LIST_H
#define __LIST_H

// http://isis.poly.edu/kulesh/stuff/src/klist/

#include <malloc.h>

/* This file is from Linux Kernel (include/linux/list.h) 
 * and modified by simply removing hardware prefetching of list items. 
 * (And some other extra features)
 * Here by copyright, credits attributed to wherever they belong.
 * Kulesh Shanmugasundaram (kulesh [squiggly] isis.poly.edu)
 */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

typedef struct list_head_s {
	struct list_head_s *next, *prev;
} list_head;

#define LIST_HEAD_INIT(ptr) { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
}

#define NEW(type)	((type*)malloc(sizeof(type)))
#define DEL(var)	free(var)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(list_head *new,
			      list_head *prev,
			      list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(list_head *list, list_head *new_entry)
{
	__list_add(new_entry, list, list->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(list_head *list, list_head *new_entry)
{
	__list_add(new_entry, list->prev, list);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(list_head *prev, list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

/**
 * list_del - deletes and frees entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
#define list_del_free(entry, member) \
	list_del(&(entry->member)); \
	DEL(entry);

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(list_head *entry)
{
	__list_del(entry->prev, entry->next);
	LIST_HEAD_INIT(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(list_head *dest, list_head *entry)
{
        __list_del(entry->prev, entry->next);
        list_add(entry, dest);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(list_head *list,
				  list_head *entry)
{
        __list_del(entry->prev, entry->next);
        list_add_tail(entry, list);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(list_head *list,
				 list_head *head)
{
	list_head *first = list->next;
	list_head *last = list->prev;
	list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(list_head *dest, list_head *src)
{
	if (!list_empty(src))
		__list_splice(src, dest);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(list_head *dest,
				    list_head *src)
{
	if (!list_empty(src)) {
		__list_splice(src, dest);
		LIST_HEAD_INIT(src);
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each	-	iterate over a list
 * @list:	the list structure
 * @member:	the name of the list_head element inside of the list
 *
 * entry will be the current list element.
 */
#define list_for_each(list, member) \
	for (	typeof(list) entry = list_entry((list)->member.next, typeof(*(list)), member);	\
			&entry->member != &(list)->member; \
			entry = list_entry(entry->member.next, typeof(*(entry)), member))

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @list:	the list structure
 * @member:	the name of the list_head element inside of the list
 */
#define list_for_each_prev(list, member) \
		for (	typeof(list) entry = list_entry((list)->member.prev, typeof(*(list)), member);	\
				&entry->member != &(list)->member; \
				entry = list_entry(entry->member.prev, typeof(*(entry)), member))

/**
 * list_for_each_safe	-	iterate over a list safe against removal of list entry
* @list:	the list structure
* @member:	the name of the list_head element inside of the list
*/
#define list_for_each_safe(list, member) \
	for (	typeof((list)) entry = list_entry((list)->member.next, typeof(*(list)), member),	\
			next_entry = list_entry(entry->member.next, typeof(*(list)), member);	\
			&entry->member != &((list)->member); \
			entry = next_entry, next_entry = list_entry(next_entry->member.next, typeof(*(list)), member))

/**
 * list_for_each_safe_prev	-	iterate over a list safe against removal of list entry backwards
* @list:	the list structure
* @member:	the name of the list_head element inside of the list
*/
#define list_for_each_safe_prev(list, member) \
		for (	typeof((list)) entry = list_entry((list)->member.prev, typeof(*(list)), member),	\
				next_entry = list_entry(entry->member.next, typeof(*(list)), member);	\
				&entry->member != &((list)->member); \
				entry = next_entry, next_entry = list_entry(next_entry->member.prev, typeof(*(list)), member))

#define DEL_LIST(list, member) \
	list_for_each_safe(list, member) \
		DEL(entry);
#endif
