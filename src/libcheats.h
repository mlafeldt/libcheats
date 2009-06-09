#ifndef _READCHEATS_H_
#define _READCHEATS_H_

#include "codelist.h"

/* Version information */
#define LIBCHEATS_VERSION	0x0100	/* libcheats 1.0 */
#define LIBCHEATS_VERSION_MAJOR	1
#define LIBCHEATS_VERSION_MINOR	0


#define CHEATS_TRUE		0
#define CHEATS_FALSE		(-1)

typedef struct {
	gamelist_t	games;
	void		(*destructor)(void *);
	int		flags;
	const char	*error_text;
	int		error_line;
} cheats_t;

extern void cheats_init(cheats_t *cheats);
extern void cheats_destroy(cheats_t *cheats);

extern int cheats_read(cheats_t *cheats, FILE *stream);
extern int cheats_read_file(cheats_t *cheats, const char *filename);
extern int cheats_read_buf(cheats_t *cheats, const char *buf);

extern int cheats_write(const cheats_t *cheats, FILE *stream);
extern int cheats_write_file(const cheats_t *cheats, const char *filename);

extern const char *cheats_error_text(const cheats_t *cheats);
extern int cheats_error_line(const cheats_t *cheats);

#endif /* _READCHEATS_H_ */
