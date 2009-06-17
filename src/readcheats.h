#ifndef _READCHEATS_H_
#define _READCHEATS_H_

#include "cheatlist.h"

/* Max line length to parse */
#define LINE_MAX	255

/* Number of digits per cheat code */
#define CODE_DIGITS	16

/* Tokens used for parsing */
#define TOK_NO		0
#define TOK_GAME_TITLE	1
#define TOK_CHEAT_DESC	2
#define TOK_CHEAT_CODE	4

/**
 * parser_ctx_t - parser context
 * @top: token at the top of each game, see TOK_* flags
 * @next: next expected token(s), see TOK_* flags
 * @game: ptr to current game
 * @cheat: ptr to current cheat
 * @code: ptr to current code
 */
typedef struct _parser_ctx {
	int	top;
	int	next;
	game_t	*game;
	cheat_t	*cheat;
	code_t	*code;
} parser_ctx_t;

void init_parser(parser_ctx_t *ctx);
int parse_line(const char *line, int nl, parser_ctx_t *ctx, cheats_t *cheats);

#endif /* _READCHEATS_H_ */
