#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"
#include "codelist.h"
#include "gameid.h"
#include "codeman.h"
#include "dbgprintf.h"

/* Debug stuff */
/*
#define _DBG_TOK
#define _DBG_ID
*/
/* Max line length to parse, rest is cut off! */
#define LINE_MAX	255

/* Start of game id string */
#define ID_START	"@ID"

/* Number of digits per cheat code */
#define CODE_DIGITS	16

/* Tokens used for parsing */
#define TOK_GAME_ID	1
#define TOK_GAME_TITLE	2
#define TOK_CHEAT_DESC	4
#define TOK_CHEAT_CODE	8

/* Separator inserted between games when writing to text file */
#define GAME_SEP	"\n//--------\n\n"

/*
 * tok2str - Converts a token value to a string.
 */
static const char *tok2str(int tok)
{
	static const char *str[] = {
		"game id",
		"game title",
		"cheat description",
		"cheat code"
	};

	switch (tok) {
	case TOK_GAME_ID:
		return str[0];
	case TOK_GAME_TITLE:
		return str[1];
	case TOK_CHEAT_DESC:
		return str[2];
	case TOK_CHEAT_CODE:
		return str[3];
	default:
		return NULL;
	}
}

/*
 * is_game_id - Returns 1 if @s indicates a game ID.
 *
 * Example: @ID SLES_123.45 2714195 -
 */
static int is_game_id(const char *s)
{
	size_t len = strlen(ID_START);

	/* get_id() checks the string more carefully */
	return (strlen(s) >= len) && !strncmp(s, ID_START, len);
}

/*
 * is_game_title - Returns 1 if @s indicates a game title.
 *
 * Example: "TimeSplitters PAL"
 */
static int is_game_title(const char *s)
{
	size_t len = strlen(s);

	return ((len >= 2) && (*s == '"') && (s[len - 1] == '"'));
}

/*
 * is_cheat_code - Returns 1 if @s indicates a cheat code.
 *
 * Example: 10B8DAFA 00003F00
 */
static int is_cheat_code(const char *s)
{
	int i = 0;

	while (*s) {
		if (isxdigit(*s)) {
			if (++i > CODE_DIGITS)
				return 0;
		}
		else if (!isspace(*s)) {
			return 0;
		}
		s++;
	}

	return (i == CODE_DIGITS);
}

/*
 * get_token - Gets the token value TOK_* for string @s.
 */
static int get_token(const char *s, int top)
{
	if ((top == TOK_GAME_ID) && is_game_id(s))
		return TOK_GAME_ID;
	else if (is_game_title(s))
		return TOK_GAME_TITLE;
	else if (is_cheat_code(s))
		return TOK_CHEAT_CODE;
	else
		return TOK_CHEAT_DESC;
}

/*
 * next_token - Returns the next expected token(s).
 */
int next_token(int tok, int top)
{
	switch (tok) {
	case TOK_GAME_ID:
		return TOK_GAME_ID | TOK_GAME_TITLE;
	case TOK_GAME_TITLE:
		return top | TOK_CHEAT_DESC;
	case TOK_CHEAT_DESC:
	case TOK_CHEAT_CODE:
		return top | TOK_CHEAT_DESC | TOK_CHEAT_CODE;
	default:
		return 0;
	}
}

/*
 * get_id - Parses the string @s for the game ID.
 */
static int get_id(const char *s, game_id_t *id)
{
	const char *sep = " \t"; /* Tokens are separated by whitespace */
	char buf[LINE_MAX + 1];
	char *p = NULL;
	int i = 0;

	memset(id, 0, sizeof(game_id_t));

	/* strtok() corrupts s, make a copy */
	strncpy(buf, s + strlen(ID_START), sizeof(buf));
	p = strtok(buf, sep);

	/* Process tokens */
	while (p != NULL && i < 3) {
		if (p[0] != '-') { /* '-' means that the value is not set */
			switch (i) {
			case 0: /* name */
				strncpy(id->name, p, GID_NAME_MAX);
				to_upper_str(id->name);
				id->set |= GID_F_NAME;
#ifdef _DBG_ID
				D_PRINTF(".name %s\n", id->name);
#endif
				break;

			case 1: /* size */
				if (!is_dec_str(p))
					return -1;
				if (!sscanf(p, "%i", &id->size))
					return -1;
				id->set |= GID_F_SIZE;
#ifdef _DBG_ID
				D_PRINTF(".size %i\n", id->size);
#endif
				break;

			case 2: /* hash */
				if (strlen(p) != (GID_HASH_SIZE * 2) || !is_hex_str(p))
					return -1;
				xstr_to_bytes(p, GID_HASH_SIZE, id->hash);
				id->set |= GID_F_HASH;
#ifdef _DBG_ID
				D_PRINTF(".hash %s\n", p);
#endif
				break;
			}
		}

		i++;
		p = strtok(NULL, sep);
	}

	/*if (!id->set)
		return -1;*/

	return 0;
}

/*
 * id2str - Converts a game ID to a string.
 */
static char *id2str(const game_id_t *id)
{
	static char str[LINE_MAX + 1];
	int i;

	i = sprintf(str, ID_START" %s ", (id->set & GID_F_NAME) ? id->name : "-");

	if (id->set & GID_F_SIZE)
		i += sprintf(str + i, "%i ", id->size);
	else
		i += sprintf(str + i, "- ");

	if (id->set & GID_F_HASH)
		bytes_to_xstr(id->hash, GID_HASH_SIZE, str + i);
	else
		sprintf(str + i, "-");

	return str;
}

/*
 * get_game - Creates a game_t struct from game title and ID.
 */
static game_t *get_game(const char *title, const game_id_t *id)
{
	char buf[CL_TITLE_MAX + 1];

	/* Remove leading and trailing quotes from game title */
	strncpy(buf, title + 1, strlen(title) - 2);
	buf[strlen(title) - 2] = 0;

	return mkgame(id, buf, NULL, 0);
}

/*
 * get_cheat - Creates a cheat_t struct from a cheat description.
 */
static cheat_t *get_cheat(const char *desc)
{
	return mkcheat(desc, NULL, 0);
}

/*
 * get_code - Creates a code_t struct from string @s.
 */
static code_t *get_code(const char *s)
{
	char digits[CODE_DIGITS];
	int i = 0;
	u32 addr, val;

	while (*s) {
		if (isxdigit(*s))
			digits[i++] = *s;
		s++;
	}

	sscanf(digits, "%08X%08X", &addr, &val);

	return mkcode(addr, val, 0);
}

/*
 * is_cmt_str - Returns 1 if @s indicates a comment.
 */
static int is_cmt_str(const char *s)
{
	return (strlen(s) >= 2 && !strncmp(s, "//", 2)) || (*s == '#');
}

/**
 * parser_ctx_t - parser context
 * @source: source of codes, e.g. filename
 * @top: token at the top of each game, see TOK_* flags
 * @next: next expected token(s), see TOK_* flags
 * @id: current game id
 * @game: ptr to current game
 * @cheat: ptr to current cheat
 * @code: ptr to current code
 */
typedef struct _parser_ctx {
	const char *source;
	int top;
	int next;
	game_id_t id;
	game_t *game;
	cheat_t *cheat;
	code_t *code;
} parser_ctx_t;

/*
 * init_parser - Initializes the parser's context.  Must be called each time
 * before a file is parsed.
 */
static void init_parser(parser_ctx_t *ctx, const char *source, int top)
{
	if (ctx != NULL) {
		ctx->source = source;
		ctx->top = top;
		ctx->next = top;
		memset(&ctx->id, 0, sizeof(game_id_t));
		ctx->game = NULL;
		ctx->cheat = NULL;
		ctx->code = NULL;
	}
}

/*
 * parse_err - Prints out information about a parse error.
 */
static int parse_err(const parser_ctx_t *ctx, int nl, const char *msg, ...)
{
	va_list ap;

	printf("%s:%i: error: ", ctx->source, nl);
	va_start(ap, msg);
	vprintf(msg, ap);
	va_end(ap);

	return -1;
}

/*
 * parse_line - Parses the current line and adds the found token to the @list.
 */
static int parse_line(const char *line, int nl, parser_ctx_t *ctx, gamelist_t *list)
{
	int tok = get_token(line, ctx->top);
#ifdef _DBG_TOK
	D_PRINTF("(%i) %i %s\n", nl, tok, line);
#endif
	/* Check if current token is expected; makes sure that the list
	 * operations succeed.
	 */
	if (!(ctx->next & tok)) {
		int x = 0;
		if (ctx->next & TOK_CHEAT_CODE)
			x = TOK_CHEAT_CODE;
		else if (ctx->next & TOK_CHEAT_DESC)
			x = TOK_CHEAT_DESC;
		else if (ctx->next & TOK_GAME_TITLE)
			x = TOK_GAME_TITLE;
		else if (ctx->next & TOK_GAME_ID)
			x = TOK_GAME_ID;
		return parse_err(ctx, nl, "%s invalid here; %s expected\n",
			tok2str(tok), tok2str(x));
	}

	/* Process actual token and add it to the list it belongs to. */
	switch (tok) {
	case TOK_GAME_ID:
		if (get_id(line, &ctx->id) < 0)
			return parse_err(ctx, nl, "invalid game id\n");
		break;

	case TOK_GAME_TITLE:
		ctx->game = get_game(line, &ctx->id);
		if (ctx->game == NULL)
			return parse_err(ctx, nl, "mem alloc failure in get_game()\n");
		cl_add(list, ctx->game);
		break;

	case TOK_CHEAT_DESC:
		ctx->cheat = get_cheat(line);
		if (ctx->cheat == NULL)
			return parse_err(ctx, nl, "mem alloc failure in get_cheat()\n");
		cl_add(&ctx->game->cheats, ctx->cheat);
		break;

	case TOK_CHEAT_CODE:
		ctx->code = get_code(line);
		if (ctx->code == NULL)
			return parse_err(ctx, nl, "mem alloc failure in get_code()\n");
		cl_add(&ctx->cheat->codes, ctx->code);
		break;
	}

	ctx->next = next_token(tok, ctx->top);
	return 0;
}

/**
 * codes_from_textfile - Parses a text file for codes and adds them to a list.
 * @filename: name of text file to parse
 * @list: list to add codes to
 * @opt: flag CODES_OPT_GID: text is expected to contain game ids
 * @return: 0: success, <0: error
 *
 * Uses fgets() to read each line from the text file. Should not be used on the
 * PS2 as file I/O operations are very slow there.
 */
int codes_from_textfile(const char *filename, gamelist_t *list, int opt)
{
	FILE *fp;
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;
	int ret = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return -1;

	init_parser(&ctx, filename, (opt & CODES_OPT_GID) ?
		TOK_GAME_ID : TOK_GAME_TITLE);

	while (fgets(line, sizeof(line), fp) != NULL) { /* Scanner */
		if (!is_empty_str(line)) {
			/* Screener */
			term_str(line, is_cmt_str);
			trim_str(line);
			if (strlen(line) > 0) {
				/* Parser */
				ret = parse_line(line, nl, &ctx, list);
				if (ret < 0) break; /* return on error */
			}
		}
		nl++;
	}

	fclose(fp);
	return ret;
}

/**
 * codes_from_textbuf - Parses a text buffer for codes and adds them to a list.
 * @buf: buffer holding text (must be NUL-terminated!)
 * @source: source of text that is parsed, e.g. filename
 * @list: list to add codes to
 * @opt: flag CODES_OPT_GID: text is expected to contain game ids
 * @return: 0: success, <0: error
 *
 * On PS2, this is the preferred way to read codes from a text file as file I/O
 * operations are very slow.
 */
int codes_from_textbuf(const char *buf, const char *source, gamelist_t *list, int opt)
{
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;
	int ret = 0;

	init_parser(&ctx, source, (opt & CODES_OPT_GID) ?
		TOK_GAME_ID : TOK_GAME_TITLE);

	while (*buf) {
		/* Scanner */
		int len = chr_idx(buf, LF);
		if (len < 0)
			len = strlen(line);
		if (len > LINE_MAX)
			len = LINE_MAX;

		if (!is_empty_substr(buf, len)) {
			strncpy(line, buf, len);
			line[len] = NUL;

			/* Screener */
			term_str(line, is_cmt_str);
			trim_str(line);
			if (strlen(line) > 0) {
				/* Parser */
				ret = parse_line(line, nl, &ctx, list);
				if (ret < 0) break; /* return on error */
			}
		}
		nl++;
		buf += len + 1;
	}

	return ret;
}

/**
 * codes_to_textfile - Writes all codes in a list to a text file.
 * @filename: name of file to write codes to
 * @list: list with codes
 * @opt: flag CODES_OPT_GID: also write game ids to text file
 * @return: 0: success, <0: error
 */
int codes_to_textfile(const char *filename, const gamelist_t *list, int opt)
{
	FILE *fp;
	game_t *game;
	cheat_t *cheat;
	code_t *code;

	fp = fopen(filename, "w");
	if (fp == NULL)
		return -1;

	for (game = list->head; game != NULL; game = game->next) {
		if (opt & CODES_OPT_GID)
			fprintf(fp, "%s\n", id2str(&game->id));
		fprintf(fp, "\"%s\"\n", game->title);
		for (cheat = game->cheats.head; cheat != NULL; cheat = cheat->next) {
			fprintf(fp, "%s\n", cheat->desc);
			for (code = cheat->codes.head; code != NULL; code = code->next) {
				fprintf(fp, "%08X %08X\n", code->addr, code->val);
			}
		}
		fprintf(fp, GAME_SEP);
	}

	fclose(fp);
	return 0;
}
