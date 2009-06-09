#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"
#include "codelist.h"
#include "readcheats.h"
#include "dbgprintf.h"

/* Debug stuff */
/*
#define _DBG_TOK
#define _DBG_ID
*/
/* Max line length to parse, rest is cut off! */
#define LINE_MAX	255

/* Number of digits per cheat code */
#define CODE_DIGITS	16

/* Tokens used for parsing */
#define TOK_GAME_TITLE	1
#define TOK_CHEAT_DESC	2
#define TOK_CHEAT_CODE	4

/* Separator inserted between games when writing to text file */
#define GAME_SEP	"\n//--------\n\n"

/*
 * tok2str - Converts a token value to a string.
 */
static const char *tok2str(int tok)
{
	static const char *str[] = {
		"game title",
		"cheat description",
		"cheat code"
	};

	switch (tok) {
	case TOK_GAME_TITLE:
		return str[0];
	case TOK_CHEAT_DESC:
		return str[1];
	case TOK_CHEAT_CODE:
		return str[2];
	default:
		return NULL;
	}
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
	if (is_game_title(s))
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
 * get_game - Creates a game_t struct from game title.
 */
static game_t *get_game(const char *title)
{
	char buf[CL_TITLE_MAX + 1];

	/* Remove leading and trailing quotes from game title */
	strncpy(buf, title + 1, strlen(title) - 2);
	buf[strlen(title) - 2] = 0;

	return mkgame(buf, NULL, 0);
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
 * @game: ptr to current game
 * @cheat: ptr to current cheat
 * @code: ptr to current code
 */
typedef struct _parser_ctx {
	const char *source;
	int top;
	int next;
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
		return parse_err(ctx, nl, "%s invalid here; %s expected\n",
			tok2str(tok), tok2str(x));
	}

	/* Process actual token and add it to the list it belongs to. */
	switch (tok) {
	case TOK_GAME_TITLE:
		ctx->game = get_game(line);
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
 * cheats_init - Initialize a cheats object.
 * @cheats: cheats
 */
void cheats_init(cheats_t *cheats)
{
	if (cheats != NULL) {
		memset(cheats, 0, sizeof(cheats_t));
		cl_init(&cheats->games);
	}
}

/**
 * cheats_destroy - Destroy a cheats object, deallocate all memory associated
 * with it, but not including the cheats_t structure itself.
 * @cheats: cheats
 */
void cheats_destroy(cheats_t *cheats)
{
	if (cheats != NULL) {
		cl_free(&cheats->games);
	}
}

/**
 * cheats_read - Read cheats from a stream.
 * @cheats: cheats
 * @stream: stream to read cheats from
 * @return: 0: success, <0: error
 */
int cheats_read(cheats_t *cheats, FILE *stream)
{
	return 0;
}

/**
 * cheats_read_file - Read cheats from a text file.
 * @cheats: cheats
 * @filename: name of file to read cheats from
 * @return: 0: success, <0: error
 */
int cheats_read_file(cheats_t *cheats, const char *filename)
{
	FILE *fp;
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;
	int ret = 0;

	if (cheats == NULL || filename == NULL)
		return -1;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return -1;

	init_parser(&ctx, filename, TOK_GAME_TITLE);

	while (fgets(line, sizeof(line), fp) != NULL) { /* Scanner */
		if (!is_empty_str(line)) {
			/* Screener */
			term_str(line, is_cmt_str);
			trim_str(line);
			if (strlen(line) > 0) {
				/* Parser */
				ret = parse_line(line, nl, &ctx, &cheats->games);
				if (ret < 0) break; /* return on error */
			}
		}
		nl++;
	}

	fclose(fp);
	return ret;
}

/**
 * cheats_read_buf - Read cheats from a text buffer.
 * @cheats: cheats
 * @buf: buffer holding text (must be NUL-terminated!)
 * @return: 0: success, <0: error
 */
int cheats_read_buf(cheats_t *cheats, const char *buf)
{
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;
	int ret = 0;

	if (cheats == NULL || buf == NULL)
		return -1;

	init_parser(&ctx, "buffer", TOK_GAME_TITLE);

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
				ret = parse_line(line, nl, &ctx, &cheats->games);
				if (ret < 0) break; /* return on error */
			}
		}
		nl++;
		buf += len + 1;
	}

	return ret;
}

/**
 * cheats_write - Write cheats to a stream.
 * @cheats: cheats
 * @stream: stream to write cheats to
 * @return: 0: success, <0: error
 */
int cheats_write(const cheats_t *cheats, FILE *stream)
{
	return 0;
}

/**
 * cheats_write_file - Write cheats to a text file.
 * @cheats: cheats
 * @filename: name of file to write cheats to
 * @return: 0: success, <0: error
 */
int cheats_write_file(const cheats_t *cheats, const char *filename)
{
	FILE *fp;
	game_t *game;
	cheat_t *cheat;
	code_t *code;

	if (cheats == NULL || filename == NULL)
		return -1;

	fp = fopen(filename, "w");
	if (fp == NULL)
		return -1;

	for (game = cheats->games.head; game != NULL; game = game->next) {
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
