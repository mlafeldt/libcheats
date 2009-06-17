/*
 * cheatlist.c - Cheat list handling
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

#include "mytypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cheatlist.h"

/**
 * build_game - Create a new game object and populate it.
 * @title: game title
 * @cheats: game cheats
 * @return: ptr to new game object, or NULL on mem alloc error
 */
game_t *build_game(const char *title, const cheatlist_t *cheats)
{
	game_t *game = (game_t*)calloc(1, sizeof(game_t));

	if (game != NULL) {
		if (title != NULL)
			strncpy(game->title, title, CL_TITLE_MAX);
		if (cheats != NULL)
			game->cheats = *cheats;
	}

	return game;
}

/**
 * build_cheat - Create a new cheat object and populate it.
 * @desc: cheat description
 * @codes: cheat codes
 * @return: ptr to new cheat object, or NULL on mem alloc error
 */
cheat_t *build_cheat(const char *desc, const codelist_t *codes)
{
	cheat_t *cheat = (cheat_t*)calloc(1, sizeof(cheat_t));

	if (cheat != NULL) {
		if (desc != NULL)
			strncpy(cheat->desc, desc, CL_DESC_MAX);
		if (codes != NULL)
			cheat->codes = *codes;
	}

	return cheat;
}

/**
 * build_code - Create a new code object.
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 * @return: ptr to new code object, or NULL on mem alloc error
 */
code_t *build_code(u32 addr, u32 val)
{
	code_t *code = (code_t*)calloc(1, sizeof(code_t));

	if (code != NULL) {
		code->addr = addr;
		code->val = val;
	}

	return code;
}

/**
 * free_codes - Free a code list.
 * @list: list to be freed
 */
void free_codes(codelist_t *list)
{
	if (list != NULL)
		list_free(list);
}

/**
 * free_cheats - Free a cheat list.
 * @list: list to be freed
 */
void free_cheats(cheatlist_t *list)
{
	cheat_t *cheat;

	if (list == NULL)
		return;

	for (cheat = list->head; cheat != NULL; cheat = cheat->next)
		free_codes(&cheat->codes);

	list_free(list);
}

/**
 * free_games - Free a game list.
 * @list: list to be freed
 */
void free_games(gamelist_t *list)
{
	game_t *game;

	if (list == NULL)
		return;

	for (game = list->head; game != NULL; game = game->next)
		free_cheats(&game->cheats);

	list_free(list);
}

/**
 * sort_games - Sort a game list by title.
 * @list: list to be sorted
 */
void sort_games(gamelist_t *list)
{
	/* TODO */
}

/**
 * find_game_by_title - Search a game list for a game with a certain title.
 * @title: title to search for
 * @list: list that is searched
 * @return: ptr to found game object, or NULL if it could not be found
 */
game_t *find_game_by_title(const char *title, const gamelist_t *list)
{
	if (title != NULL && list != NULL) {
		game_t *game = list->head;

		while (game != NULL) {
			if (!strcmp(game->title, title))
				return game;
			game = game->next;
		}
	}

	return NULL;

}
