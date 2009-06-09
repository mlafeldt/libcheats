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

struct rc_state {
	/* line state */

	/* global state */
	int rcstate;
};

extern int rc_init();
extern int rc_destroy();

extern int rc_read(gamelist_t *list, FILE *stream);
extern int rc_read_file(gamelist_t *list, const char *filename);
extern int rc_read_buf(gamelist_t *list, const char *buf);

extern int rc_write(const gamelist_t *list, FILE *stream);
extern int rc_write_file(const gamelist_t *list, const char *filename);

extern const char *rc_error_text();
extern int rc_error_line();

extern void rc_set_hook(void *hook);
extern void *rc_get_hook();
extern void rc_set_destructor(void (*destructor)(void*));

#endif /* _READCHEATS_H_ */
