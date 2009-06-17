#include "mytypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codelist.h"

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
 * cl_free - Free all games, cheats, and codes.
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
			list_free(&cheat->codes);
		}
		/* Free all cheats per game */
		list_free(&game->cheats);
	}
	/* Free all games */
	list_free(list);
}

/**
 * cl_find_game_by_title - Search a game list for a game with a certain title.
 * @title: game title to search for
 * @list: game list that is searched
 * @return: ptr to found game object, or NULL if it could not be found
 */
game_t *cl_find_game_by_title(const char *title, const gamelist_t *list)
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

/**
 * cl_sort - Sort a game list by title.
 * @list: game list to be sorted
 */
void cl_sort(gamelist_t *list)
{
	if (list == NULL)
		return;
	/* TODO */
}
