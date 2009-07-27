/*
 * cheatlist.h - Cheat list handling
 *
 * Copyright (C) 2009 misfire <misfire@xploderfreax.de>
 *
 * This file is part of libcheats.
 *
 * libcheats is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libcheats is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libcheats.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CHEATLIST_H_
#define _CHEATLIST_H_

#include <sys/queue.h>
#include <stdint.h>

/* Max game title length */
#define GAME_TITLE_MAX	80
/* Max cheat description length */
#define CHEAT_DESC_MAX	80

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


/*
 * Code defines.
 */
#define CODES_HEAD		TAILQ_HEAD
#define CODES_HEAD_INITIALIZER	TAILQ_HEAD_INITIALIZER
#define CODES_ENTRY		TAILQ_ENTRY
#define CODES_INIT		TAILQ_INIT
#define CODES_INSERT_HEAD	TAILQ_INSERT_HEAD
#define CODES_INSERT_TAIL	TAILQ_INSERT_TAIL
#define CODES_INSERT_AFTER	TAILQ_INSERT_AFTER
#define CODES_INSERT_BEFORE	TAILQ_INSERT_BEFORE
#define CODES_REMOVE		TAILQ_REMOVE
#define CODES_FOREACH		TAILQ_FOREACH
#define CODES_FOREACH_REVERSE	TAILQ_FOREACH_REVERSE
#define CODES_CONCAT		TAILQ_CONCAT
#define CODES_EMPTY		TAILQ_EMPTY
#define CODES_FIRST		TAILQ_FIRST
#define CODES_NEXT		TAILQ_NEXT
#define CODES_LAST		TAILQ_PREV

/**
 * code_t - a code object
 * @addr: code address
 * @val: code value
 * @tag: arbitrary information
 */
typedef struct _code {
	uint32_t	addr;
	uint32_t	val;
	uint32_t	tag;

	CODES_ENTRY(_code) node;
} code_t;

typedef CODES_HEAD(_codelist, _code) codelist_t;


/*
 * Cheat defines.
 */
#define CHEATS_HEAD		TAILQ_HEAD
#define CHEATS_HEAD_INITIALIZER	TAILQ_HEAD_INITIALIZER
#define CHEATS_ENTRY		TAILQ_ENTRY
#define CHEATS_INIT		TAILQ_INIT
#define CHEATS_INSERT_HEAD	TAILQ_INSERT_HEAD
#define CHEATS_INSERT_TAIL	TAILQ_INSERT_TAIL
#define CHEATS_INSERT_AFTER	TAILQ_INSERT_AFTER
#define CHEATS_INSERT_BEFORE	TAILQ_INSERT_BEFORE
#define CHEATS_REMOVE		TAILQ_REMOVE
#define CHEATS_FOREACH		TAILQ_FOREACH
#define CHEATS_FOREACH_REVERSE	TAILQ_FOREACH_REVERSE
#define CHEATS_CONCAT		TAILQ_CONCAT
#define CHEATS_EMPTY		TAILQ_EMPTY
#define CHEATS_FIRST		TAILQ_FIRST
#define CHEATS_NEXT		TAILQ_NEXT
#define CHEATS_LAST		TAILQ_PREV

/**
 * cheat_t - a cheat object
 * @desc: cheat description
 * @codes: cheat codes
 * @tag: arbitrary information
 */
typedef struct _cheat {
	char		desc[CHEAT_DESC_MAX + 1];
	codelist_t	codes;
	uint32_t	tag;

	CHEATS_ENTRY(_cheat) node;
} cheat_t;

typedef CHEATS_HEAD(_cheatlist, _cheat) cheatlist_t;

/*
 * Game defines.
 */
#define GAMES_HEAD		TAILQ_HEAD
#define GAMES_HEAD_INITIALIZER	TAILQ_HEAD_INITIALIZER
#define GAMES_ENTRY		TAILQ_ENTRY
#define GAMES_INIT		TAILQ_INIT
#define GAMES_INSERT_HEAD	TAILQ_INSERT_HEAD
#define GAMES_INSERT_TAIL	TAILQ_INSERT_TAIL
#define GAMES_INSERT_AFTER	TAILQ_INSERT_AFTER
#define GAMES_INSERT_BEFORE	TAILQ_INSERT_BEFORE
#define GAMES_REMOVE		TAILQ_REMOVE
#define GAMES_FOREACH		TAILQ_FOREACH
#define GAMES_FOREACH_REVERSE	TAILQ_FOREACH_REVERSE
#define GAMES_CONCAT		TAILQ_CONCAT
#define GAMES_EMPTY		TAILQ_EMPTY
#define GAMES_FIRST		TAILQ_FIRST
#define GAMES_NEXT		TAILQ_NEXT
#define GAMES_LAST		TAILQ_PREV

/**
 * game_t - a game object
 * @title: game title
 * @cheats: game cheats
 * @tag: arbitrary information
 */
typedef struct _game {
	char		title[GAME_TITLE_MAX + 1];
	cheatlist_t	cheats;
	uint32_t	tag;

	GAMES_ENTRY(_game) node;
} game_t;

typedef GAMES_HEAD(_gamelist, _game) gamelist_t;



extern code_t *make_code(uint32_t addr, uint32_t val, uint32_t tag);
extern cheat_t *make_cheat(const char *desc, codelist_t *codes, uint32_t tag);
extern game_t *make_game(const char *title, cheatlist_t *cheats, uint32_t tag);

extern void remove_code(codelist_t *list, code_t *code, int _free);
extern void remove_cheat(cheatlist_t *list, cheat_t *cheat, int _free);
extern void remove_game(gamelist_t *list, game_t *game, int _free);

extern void free_codes(codelist_t *list);
extern void free_cheats(cheatlist_t *list);
extern void free_games(gamelist_t *list);

extern game_t *find_game_by_title(const char *title, const gamelist_t *list);

#endif /* _CHEATLIST_H_ */
