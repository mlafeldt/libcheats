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

/*
 * Here's how everything is organized.  The current implementation uses tail
 * queues to store games, cheats, and codes, so the TAILQ_* macros from
 * <sys/queue.h> are your friend.
 *
 * gamelist
 * |- game
 * |  |- cheatlist
 * |  |  |- cheat
 * |  |  |  |- codelist
 * |  |  |  |  |- code
 * |  |  |  |  |- code
 * |  |  |  |  |- ...
 * |  |  |- cheat
 * |  |  |  |- codelist
 * |  |  |  |  |- code
 * |  |  |  |  |- code
 * |  |  |  |  |- ...
 * |  |  |- ...
 * |- game
 * |- ...
 */

/**
 * code_t - a code object
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 */
typedef struct _code {
	u_int32_t	addr;
	u_int32_t	val;
	u_int32_t	tag;

	TAILQ_ENTRY(_code) node;
} code_t;

typedef TAILQ_HEAD(codelist, _code) codelist_t;

/**
 * cheat_t - a cheat object
 * @desc: cheat description
 * @codes: cheat codes
 * @tag: arbitrary information
 */
typedef struct _cheat {
	char		desc[CL_DESC_MAX + 1];
	codelist_t	codes;
	u_int32_t	tag;

	TAILQ_ENTRY(_cheat) node;
} cheat_t;

typedef TAILQ_HEAD(cheatlist, _cheat) cheatlist_t;

/**
 * game_t - a game object
 * @title: game title
 * @cheats: game cheats
 * @tag: arbitrary information
 */
typedef struct _game {
	char		title[CL_TITLE_MAX + 1];
	cheatlist_t	cheats;
	u_int32_t	tag;

	TAILQ_ENTRY(_game) node;
} game_t;

typedef TAILQ_HEAD(gamelist, _game) gamelist_t;

extern code_t *make_code(u_int32_t addr, u_int32_t val, u_int32_t tag);
extern cheat_t *make_cheat(const char *desc, codelist_t *codes, u_int32_t tag);
extern game_t *make_game(const char *title, cheatlist_t *cheats, u_int32_t tag);

extern void remove_code(codelist_t *list, code_t *code);
extern void remove_cheat(cheatlist_t *list, cheat_t *cheat);
extern void remove_game(gamelist_t *list, game_t *game);

extern void free_codes(codelist_t *list);
extern void free_cheats(cheatlist_t *list);
extern void free_games(gamelist_t *list);

extern game_t *find_game_by_title(const char *title, const gamelist_t *list);

#endif /* _CHEATLIST_H_ */
