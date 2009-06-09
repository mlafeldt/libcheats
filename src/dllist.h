/*
 * dllist.h - Double linked list implementation
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

#ifndef _DLLIST_H_
#define _DLLIST_H_

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

void list_init(void *list);
int list_is_empty(const void *list);
void list_add(void *list, void *node);
void list_insert(void *list, void *node, void *after);
void *list_remove(void *list, void *node);
void list_clear(void *list);
void list_free(void *list);

#endif /*_DLLIST_H_*/
