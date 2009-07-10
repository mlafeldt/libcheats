/*
 * cheatlist.h - Cheat list handling
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

#ifndef _CHEATLIST_H_
#define _CHEATLIST_H_

#include <sys/types.h>
#include <sys/queue.h>

/* Max game title length */
#define CL_TITLE_MAX	127
/* Max cheat description length */
#define CL_DESC_MAX	127

/**
 * code_t - a code object
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 * @node:
 */
struct code {
	u_int32_t addr;
	u_int32_t val;
	u_int32_t tag;

	TAILQ_ENTRY(code) node;
};
typedef struct code code_t;

/* struct codelist */
TAILQ_HEAD(codelist, code);
typedef struct codelist codelist_t;

/**
 * cheat_t - a cheat object
 * @desc: cheat description
 * @codes: cheat codes
 * @tag: arbitrary information
 */
struct cheat {
	char desc[CL_DESC_MAX + 1];
	struct codelist codes;
	u_int32_t tag;

	TAILQ_ENTRY(cheat) node;
};
typedef struct cheat cheat_t;

/* struct cheatlist */
TAILQ_HEAD(cheatlist, cheat);
typedef struct cheatlist cheatlist_t;

/**
 * game_t - a game object
 * @next: next game in list
 * @prev: previous game in list
 * @title: game title
 * @cheats: game cheats
 * @tag: arbitrary information
 */
struct game {
	char title[CL_TITLE_MAX + 1];
	struct cheatlist cheats;
	u_int32_t tag;

	TAILQ_ENTRY(game) node;
};
typedef struct game game_t;

/* struct gamelist */
TAILQ_HEAD(gamelist, game);
typedef struct gamelist gamelist_t;

extern game_t *build_game(const char *title, const cheatlist_t *cheats);
extern cheat_t *build_cheat(const char *desc, const codelist_t *codes);
extern code_t *build_code(u_int32_t addr, u_int32_t val);

extern void free_codes(codelist_t *list);
extern void free_cheats(cheatlist_t *list);
extern void free_games(gamelist_t *list);

extern void sort_games(gamelist_t *list);

extern game_t *find_game_by_title(const char *title, const gamelist_t *list);

#endif /* _CHEATLIST_H_ */
