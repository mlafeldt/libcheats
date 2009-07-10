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

#include <sys/types.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <string.h>
#include "cheatlist.h"
#include "dbgprintf.h"

/**
 * make_code - Create a new code object.
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 * @return: ptr to new code object, or NULL on mem alloc error
 */
code_t *make_code(u_int32_t addr, u_int32_t val, u_int32_t tag)
{
	code_t *code = (code_t*)malloc(sizeof(code_t));

	if (code != NULL) {
		code->addr = addr;
		code->val = val;
		code->tag = tag;
	}

	return code;
}

/**
 * make_cheat - Create a new cheat object and populate it.
 * @desc: cheat description
 * @codes: cheat codes
 * @tag: arbitrary information
 * @return: ptr to new cheat object, or NULL on mem alloc error
 */
cheat_t *make_cheat(const char *desc, const codelist_t *codes, u_int32_t tag)
{
	cheat_t *cheat = (cheat_t*)malloc(sizeof(cheat_t));

	if (cheat != NULL) {
		if (desc != NULL)
			strncpy(cheat->desc, desc, CL_DESC_MAX);
		if (codes != NULL)
			cheat->codes = *codes;
		else
			TAILQ_INIT(&cheat->codes);
		cheat->tag = tag;
	}

	return cheat;
}

/**
 * make_game - Create a new game object and populate it.
 * @title: game title
 * @cheats: game cheats
 * @tag: arbitrary information
 * @return: ptr to new game object, or NULL on mem alloc error
 */
game_t *make_game(const char *title, const cheatlist_t *cheats, u_int32_t tag)
{
	game_t *game = (game_t*)malloc(sizeof(game_t));

	if (game != NULL) {
		if (title != NULL)
			strncpy(game->title, title, CL_TITLE_MAX);
		if (cheats != NULL)
			game->cheats = *cheats;
		else
			TAILQ_INIT(&game->cheats);
		game->tag = tag;
	}

	return game;
}

/**
 * free_codes - Free a code list.
 * @list: list to be freed
 */
void free_codes(codelist_t *list)
{
	code_t *code;

	while ((code = TAILQ_FIRST(list)) != NULL) {
		D_PRINTF("free %08X %08X\n", code->addr, code->val);
		TAILQ_REMOVE(list, code, node);
		free(code);
	}
}

/**
 * free_cheats - Free a cheat list.
 * @list: list to be freed
 */
void free_cheats(cheatlist_t *list)
{
	cheat_t *cheat;

	while ((cheat = TAILQ_FIRST(list)) != NULL) {
		D_PRINTF("free %s\n", cheat->desc);
		free_codes(&cheat->codes);
		TAILQ_REMOVE(list, cheat, node);
		free(cheat);
	}
}

/**
 * free_games - Free a game list.
 * @list: list to be freed
 */
void free_games(gamelist_t *list)
{
	game_t *game;

	while ((game = TAILQ_FIRST(list)) != NULL) {
		D_PRINTF("free %s\n", game->title);
		free_cheats(&game->cheats);
		TAILQ_REMOVE(list, game, node);
		free(game);
	}
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
	game_t *game;

	TAILQ_FOREACH(game, list, node) {
		if (!strcmp(game->title, title))
			return game;
	}

	return NULL;
}
