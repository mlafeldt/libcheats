/*
 * readcheats.h - Read cheat codes from text files
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

#ifndef _READCHEATS_H_
#define _READCHEATS_H_

#include <stdio.h>
#include "cheatlist.h"

extern char parse_error_text[256];
extern int parse_error_line;

extern int parse_stream(gamelist_t *list, FILE *stream);
extern int parse_buf(gamelist_t *list, const char *buf);

#endif /* _READCHEATS_H_ */
