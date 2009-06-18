/*
 * libcheats.h - Library for reading, manipulating, and writing cheat codes in
 * text format
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

#ifndef _LIBCHEATS_H_
#define _LIBCHEATS_H_

#include <stdio.h>
#include "cheatlist.h"

/* Version information */
#define LIBCHEATS_VERSION	"1.0"
#define LIBCHEATS_VERNUM	0x0100
#define LIBCHEATS_VER_MAJOR	1
#define LIBCHEATS_VER_MINOR	0

/* Return values */
#define CHEATS_TRUE		0
#define CHEATS_FALSE		(-1)

/**
 * cheats_t - cheats object
 * @game:
 * @source:
 * @error_text:
 * @error_line:
 */
typedef struct _cheats {
	gamelist_t	games;
	char		source[256];
	char		error_text[256];
	int		error_line;
} cheats_t;

extern void cheats_init(cheats_t *cheats);
extern void cheats_destroy(cheats_t *cheats);

extern int cheats_read(cheats_t *cheats, FILE *stream);
extern int cheats_read_file(cheats_t *cheats, const char *filename);
extern int cheats_read_buf(cheats_t *cheats, const char *buf);

extern int cheats_write(cheats_t *cheats, FILE *stream);
extern int cheats_write_file(cheats_t *cheats, const char *filename);

extern const char *cheats_error_text(const cheats_t *cheats);
extern int cheats_error_line(const cheats_t *cheats);

extern game_t *cheats_find_game(const cheats_t *cheats, const char *title);

extern game_t *cheats_add_game(cheats_t *cheats, const game_t *game);

extern int cheats_remove_game(cheats_t *cheats, const game_t *game);

#endif /* _LIBCHEATS_H_ */
