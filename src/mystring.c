/*
 * mystring.c - Small library of string functions
 *
 * Copyright (C) 2009-2012 Mathias Lafeldt <mathias.lafeldt@gmail.com>
 *
 * This file is part of libcheats.
 *
 * libcheats is licensed under the terms of the MIT License. See LICENSE file.
 */

#include <ctype.h>
#include <string.h>
#include "mystring.h"

/*
 * chr_idx - Returns the index within @s of the first occurrence of the
 * specified char @c.  If no such char occurs in @s, then (-1) is returned.
 */
size_t chr_idx(const char *s, char c)
{
	size_t i = 0;

	while (s[i] && (s[i] != c))
		i++;

	return (s[i] == c) ? i : -1;
}

/*
 * term_str - Terminate string @s where the callback functions returns non-zero.
 */
char *term_str(char *s, int(*callback)(const char *))
{
	if (callback != NULL) {
		while (*s) {
			if (callback(s)) {
				*s = NUL;
				break;
			}
			s++;
		}
	}

	return s;
}

/*
 * trim_str - Removes white space from both ends of the string @s.
 */
int trim_str(char *s)
{
	size_t first = 0;
	size_t last;
	size_t slen;
	char *t = s;

	/* Return if string is empty */
	if (is_empty_str(s))
		return -1;

	/* Get first non-space char */
	while (isspace(*t++))
		first++;

	/* Get last non-space char */
	last = strlen(s) - 1;
	t = &s[last];
	while (isspace(*t--))
		last--;

	/* Kill leading/trailing spaces */
	slen = last - first + 1;
	memmove(s, s + first, slen);
	s[slen] = NUL;

	return slen;
}

/*
 * is_empty_str - Returns 1 if @s contains no printable chars other than white
 * space.  Otherwise, 0 is returned.
 */
int is_empty_str(const char *s)
{
	size_t slen = strlen(s);

	while (slen--) {
		if (isgraph(*s++))
			return 0;
	}

	return 1;
}

/*
 * is_empty_substr - Returns 1 if the first @count chars of @s are not printable
 * (apart from white space).  Otherwise, 0 is returned.
 */
int is_empty_substr(const char *s, size_t count)
{
	while (count--) {
		if (isgraph(*s++))
			return 0;
	}

	return 1;
}
