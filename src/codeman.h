#ifndef _CODEMAN_H_
#define _CODEMAN_H_

#include "codelist.h"

/* Options tbc. */
#define CODES_OPT_NONE	0
#define CODES_OPT_GID	1

int codes_from_textfile(const char *filename, gamelist_t *list, int opt);
int codes_from_textbuf(const char *buf, const char *source, gamelist_t *list, int opt);
int codes_to_textfile(const char *filename, const gamelist_t *list, int opt);

#endif /*_CODEMAN_H_*/
