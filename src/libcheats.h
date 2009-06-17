#ifndef _LIBCHEATS_H_
#define _LIBCHEATS_H_

#include "cheatlist.h"

/* Version information */
#define LIBCHEATS_VERSION	0x0100	/* libcheats 1.0 */
#define LIBCHEATS_VERSION_MAJOR	1
#define LIBCHEATS_VERSION_MINOR	0

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
