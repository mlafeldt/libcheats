#ifndef _READCHEATS_H_
#define _READCHEATS_H_

#include "codelist.h"

/* Version information */
#define RC_READCHEATS_VERSION	0x0100	/* Readcheats 1.0 */
#define RC_VERSION_MAJOR	1
#define RC_VERSION_MINOR	0

/* Possible state values for rc_readcheats_state */
#define RC_STATE_NONE		0x000000
#define RC_STATE_DONE		0x800000

struct readcheats_state {
	/* line state */

	/* global state */
	int rcstate;
};

/* Options tbc. */
#define RC_OPT_NONE	0

extern int rc_from_textfile(const char *filename, gamelist_t *list, int opt);
extern int rc_from_textbuf(const char *buf, const char *source, gamelist_t *list, int opt);
extern int rc_to_textfile(const char *filename, const gamelist_t *list, int opt);

#endif /* _READCHEATS_H_ */
