#include "mytypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codelist.h"

#ifdef _MEMPOOL
	/* TODO */
#else
	#define LIST_FREE(l, i)	list_free(l);
#endif /*_MEMPOOL*/

/**
 * cl_free - Removes all games, cheats, and codes from a game list and frees the
 * used memory.
 * @list: game list to be freed
 */
void cl_free(gamelist_t *list)
{
	game_t *game;
	cheat_t *cheat;

	if (list == NULL)
		return;

	for (game = list->head; game != NULL; game = game->next) {
		for (cheat = game->cheats.head; cheat != NULL; cheat = cheat->next) {
			/* Free all codes per cheat */
			LIST_FREE(&cheat->codes, g_code_pl);
		}
		/* Free all cheats per game */
		LIST_FREE(&game->cheats, g_cheat_pl);
	}
	/* Free all games */
	LIST_FREE(list, g_game_pl);
}

/**
 * mkgame - Creates a new game_t struct.
 * @id: game id
 * @title: game title
 * @cheats: game cheats
 * @tag: arbitrary information
 * @return: ptr to new game_t struct, or NULL on mem alloc error
 */
game_t *mkgame(const game_id_t *id, const char *title,
			const cheatlist_t *cheats, u32 tag)
{
#ifdef _MEMPOOL
	/* TODO */
#else
	game_t *game = (game_t*)calloc(1, sizeof(game_t));
#endif
	if (game != NULL) {
		if (id != NULL)
			/* game->id = *id; */
			memcpy(&game->id, id, sizeof(game_id_t));
		if (title != NULL)
			strncpy(game->title, title, CL_TITLE_MAX);
		if (cheats != NULL)
			game->cheats = *cheats;
		game->tag = tag;
	}

	return game;
}

/**
 * mkcheat - Creates a new cheat_t struct.
 * @desc: cheat description
 * @codes: cheat codes
 * @tag: arbitrary information
 * @return: ptr to new cheat_t struct, or NULL on mem alloc error
 */
cheat_t *mkcheat(const char *desc, const codelist_t *codes, u32 tag)
{
#ifdef _MEMPOOL
	/* TODO */
#else
	cheat_t *cheat = (cheat_t*)calloc(1, sizeof(cheat_t));
#endif
	if (cheat != NULL) {
		if (desc != NULL)
			strncpy(cheat->desc, desc, CL_DESC_MAX);
		if (codes != NULL)
			cheat->codes = *codes;
		cheat->tag = tag;
	}

	return cheat;
}

/**
 * mkcode - Creates a new code_t struct.
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 * @return: ptr to new code_t struct, or NULL on mem alloc error
 */
code_t *mkcode(u32 addr, u32 val, u32 tag)
{
#ifdef _MEMPOOL
	/* TODO */
#else
	code_t *code = (code_t*)calloc(1, sizeof(code_t));
#endif
	if (code != NULL) {
		code->addr = addr;
		code->val = val;
		code->tag = tag;
	}

	return code;
}

/**
 * cl_find_game_by_id - Searches a game list for a game with a certain id.
 * @id: game id to search for
 * @list: game list that is searched
 * @return: ptr to found game_t struct, or NULL if it could not be found
 */
game_t *cl_find_game_by_id(const game_id_t *id, const gamelist_t *list)
{
	if (list != NULL) {
		game_t *game = list->head;

		while (game != NULL) {
			if (cmp_game_id(&game->id, id) != GID_F_NONE)
				return game;
			game = game->next;
		}
	}

	return NULL;
}

/**
 * cl_find_game_by_title - Searches a game list for a game with a certain title.
 * @title: game title to search for
 * @list: game list that is searched
 * @return: ptr to found game_t struct, or NULL if it could not be found
 */
game_t *cl_find_game_by_title(const char *title, const gamelist_t *list)
{
	if (list != NULL) {
		game_t *game = list->head;

		while (game != NULL) {
			if (!strcmp(game->title, title))
				return game;
			game = game->next;
		}
	}

	return NULL;

}

/**
 * cl_print - Prints out all games, cheats, and codes of a game list.
 * @list: game list
 */
void cl_print(const gamelist_t *list)
{
	game_t *game;
	cheat_t *cheat;
	code_t *code;

	if (list == NULL)
		return;

	for (game = list->head; game != NULL; game = game->next) {
		printf("\"%s\"\n", game->title);
		for (cheat = game->cheats.head; cheat != NULL; cheat = cheat->next) {
			printf("%s\n", cheat->desc);
			for (code = cheat->codes.head; code != NULL; code = code->next) {
				printf("%08X %08X\n", code->addr, code->val);
			}
		}
	}
}

/**
 * cl_sort - Sorts a game list by game title.
 * @list: game list to be sorted
 */
void cl_sort(gamelist_t *list)
{
	if (list == NULL)
		return;
	/* TODO */
}
