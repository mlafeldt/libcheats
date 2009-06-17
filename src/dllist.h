/*
 * dllist.h - Double linked list implementation
 *
 * Based on <http://www.c.happycodings.com/Data_Structures/code3.html>
 *
 * Copyright (C) 2009 misfire <misfire@xploderfreax.de>
 *
 * This file is part of libcheats.
 *
 * libcheats is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcheats is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libcheats.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DLLIST_H_
#define _DLLIST_H_

#include <stdlib.h> /* for NULL and free() */

/**
 * node_t - node prototype
 * @next: ptr to next node in list
 * @prev: ptr to previous node in list
 *
 * A node must have two pointers like 'next' and 'prev' to be compatible.
 */
typedef struct _node {
	struct _node *next;
	struct _node *prev;
} node_t;

/**
 * list_t - list prototype
 * @head: ptr to first node of the list
 * @tail: ptr to last node of the list
 *
 * A list must have two pointers like 'head' and 'tail' to be compatible.
 */
typedef struct _list {
	void *head;
	void *tail;
} list_t;


/**
 * list_init - Initialize the list.
 * @list: list to be initialized
 */
static inline void list_init(void *list)
{
	list_t *l = (list_t*)list;

	l->head = NULL;
	l->tail = NULL;
}

/**
 * list_is_empty - Check if the list is empty.
 * @list: list
 * @return: non-zero if list is emtpy
 */
static inline int list_is_empty(const void *list)
{
	return ((list_t*)list)->head == NULL;
}

/**
 * list_add - Add a node to the end of the list.
 * @list: list
 * @node: node to be added
 */
static inline void list_add(void *list, void *node)
{
	list_t *l = (list_t*)list;
	node_t *n = (node_t*)node;

	if (l->head == NULL) {
		l->head = n;
		n->prev = NULL;
	} else {
		((node_t*)l->tail)->next = n;
		n->prev = l->tail;
	}

	l->tail = n;
	n->next = NULL;
}

/**
 * list_insert - Insert a node after another node.
 * @list: list
 * @node: node to be inserted
 * @after: node after which to insert
 */
static inline void list_insert(void *list, void *node, void *after)
{
	list_t *l = (list_t*)list;
	node_t *n = (node_t*)node;
	node_t *a = (node_t*)after;

	n->next = a->next;
	n->prev = a;

	if (a->next != NULL)
		a->next->prev = n;
	else
		l->tail = n;

	a->next = n;
}

/**
 * list_remove - Remove a node from the list.
 * @list: list
 * @node: node to be removed
 * @return: ptr to removed node
 */
static inline void *list_remove(void *list, void *node)
{
	list_t *l = (list_t*)list;
	node_t *n = (node_t*)node;

	if (n->prev == NULL)
		l->head = n->next;
	else
		n->prev->next = n->next;

	if (n->next == NULL)
		l->tail = n->prev;
	else
		n->next->prev = n->prev;

	return node;
}

/**
 * list_clear - Clear the list by removing all nodes.
 * @list: list to be cleared
 */
static inline void list_clear(void *list)
{
	list_t *l = (list_t*)list;

	while (l->head != NULL)
		list_remove(l, l->head);
}

/**
 * list_free - Remove all nodes from the list and deallocate the used memory.
 * @list: list to be freed
 */
static inline void list_free(void *list)
{
	list_t *l = (list_t*)list;

	while (l->head != NULL)
		free(list_remove(l, l->head));
}

#endif /* _DLLIST_H_ */
