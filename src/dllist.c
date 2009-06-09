/*
 * dllist.c - Double linked list implementation
 *
 * Based on <http://www.c.happycodings.com/Data_Structures/code3.html>
 *
 * Copyright (C) 2008-2009 misfire <misfire@xploderfreax.de>
 *
 * This file is part of Artemis, the PS2 game debugger.
 *
 * Artemis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Artemis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Artemis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h> /* for NULL and free() */
#include "dllist.h"

/*
 * list_init - Initializes the list.
 */
void list_init(void *list)
{
	list_t *l = (list_t*)list;

	l->head = NULL;
	l->tail = NULL;
}

/*
 * list_is_empty - Returns 1 if the list is empty.
 */
int list_is_empty(const void *list)
{
	return ((list_t*)list)->head == NULL;
}

/*
 * list_add - Adds a node to the end of the list.
 */
void list_add(void *list, void *node)
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

/*
 * list_insert - Inserts a node after another node.
 */
void list_insert(void *list, void *node, void *after)
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

/*
 * list_remove - Removes a node from the list.
 * @return: ptr to removed node
 */
void *list_remove(void *list, void *node)
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

/*
 * list_clear - Clears the list by removing all nodes.
 */
void list_clear(void *list)
{
	list_t *l = (list_t*)list;

	while (l->head != NULL)
		list_remove(l, l->head);
}

/*
 * list_free - Removes all nodes from the list and deallocates the used memory.
 */
void list_free(void *list)
{
	list_t *l = (list_t*)list;

	while (l->head != NULL)
		free(list_remove(l, l->head));
}
