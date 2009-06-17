#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "mystring.h"
#include "cheatlist.h"
#include "libcheats.h"
#include "readcheats.h"
#include "dbgprintf.h"

/**
 * cheats_init - Initialize a cheats object.
 * @cheats: cheats
 */
void cheats_init(cheats_t *cheats)
{
	if (cheats != NULL) {
		memset(cheats, 0, sizeof(cheats_t));
		list_init(&cheats->games);
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
		free_games(&cheats->games);
	}
}

/*
 * is_cmt_str - Return non-zero if @s indicates a comment.
 */
static int is_cmt_str(const char *s)
{
	return (s != NULL && strlen(s) >= 2 &&
		!strncmp(s, "//", 2)) || (*s == '#');
}

/**
 * cheats_read - Read cheats from a stream.
 * @cheats: cheats
 * @stream: stream to read cheats from
 * @return: CHEATS_TRUE: success, CHEATS_FALSE: error
 */
int cheats_read(cheats_t *cheats, FILE *stream)
{
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;

	if (cheats == NULL || stream == NULL)
		return CHEATS_FALSE;

	if (!cheats->source[0])
		strcpy(cheats->source, "-");

	init_parser(&ctx);

	setbuf(stream, NULL);

	while (fgets(line, sizeof(line), stream) != NULL) { /* Scanner */
		if (!is_empty_str(line)) {
			/* Screener */
			term_str(line, is_cmt_str);
			trim_str(line);

			/* Parser */
			if (strlen(line) > 0 && parse_line(line, nl, &ctx, cheats) < 0)
				return CHEATS_FALSE;
		}
		nl++;
	}

	return CHEATS_TRUE;
}

/**
 * cheats_read_file - Read cheats from a text file.
 * @cheats: cheats
 * @filename: name of file to read cheats from
 * @return: CHEATS_TRUE: success, CHEATS_FALSE: error
 */
int cheats_read_file(cheats_t *cheats, const char *filename)
{
	FILE *fp;
	int ret;

	if (cheats == NULL || filename == NULL)
		return CHEATS_FALSE;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		sprintf(cheats->error_text, "could not open input file %s", filename);
		return CHEATS_FALSE;
	}

	strcpy(cheats->source, filename);

	ret = cheats_read(cheats, fp);
	fclose(fp);
	return ret;
}

/**
 * cheats_read_buf - Read cheats from a text buffer.
 * @cheats: cheats
 * @buf: buffer holding text (must be NUL-terminated!)
 * @return: CHEATS_TRUE: success, CHEATS_FALSE: error
 */
int cheats_read_buf(cheats_t *cheats, const char *buf)
{
	parser_ctx_t ctx;
	char line[LINE_MAX + 1];
	int nl = 1;

	if (cheats == NULL || buf == NULL)
		return CHEATS_FALSE;

	strcpy(cheats->source, "-");
	init_parser(&ctx);

	while (*buf) {
		/* Scanner */
		int len = chr_idx(buf, LF);
		if (len < 0)
			len = strlen(line);
		else if (len > LINE_MAX)
			len = LINE_MAX;

		if (!is_empty_substr(buf, len)) {
			strncpy(line, buf, len);
			line[len] = NUL;

			/* Screener */
			term_str(line, is_cmt_str);
			trim_str(line);

			/* Parser */
			if (strlen(line) > 0 && parse_line(line, nl, &ctx, cheats) < 0)
				return CHEATS_FALSE;
		}
		nl++;
		buf += len + 1;
	}

	return CHEATS_TRUE;
}

/**
 * cheats_write - Write cheats to a stream.
 * @cheats: cheats
 * @stream: stream to write cheats to
 * @return: CHEATS_TRUE: success, CHEATS_FALSE: error
 */
int cheats_write(cheats_t *cheats, FILE *stream)
{
	game_t *game;
	cheat_t *cheat;
	code_t *code;

	if (cheats == NULL || stream == NULL)
		return CHEATS_FALSE;

	for (game = cheats->games.head; game != NULL; game = game->next) {
		fprintf(stream, "\"%s\"\n", game->title);
		for (cheat = game->cheats.head; cheat != NULL; cheat = cheat->next) {
			fprintf(stream, "%s\n", cheat->desc);
			for (code = cheat->codes.head; code != NULL; code = code->next) {
				fprintf(stream, "%08X %08X\n", code->addr, code->val);
			}
		}
		fprintf(stream, "\n//--------\n\n");
	}

	return CHEATS_FALSE;
}

/**
 * cheats_write_file - Write cheats to a text file.
 * @cheats: cheats
 * @filename: name of file to write cheats to
 * @return: CHEATS_TRUE: success, CHEATS_FALSE: error
 */
int cheats_write_file(cheats_t *cheats, const char *filename)
{
	FILE *fp;
	int ret;

	if (cheats == NULL || filename == NULL)
		return CHEATS_FALSE;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		sprintf(cheats->error_text, "could not open output file %s", filename);
		return CHEATS_FALSE;
	}

	ret = cheats_write(cheats, fp);
	fclose(fp);
	return CHEATS_TRUE;
}

/**
 * cheats_error_text - Return the text of the last parse error.
 * @cheats: cheats
 * @return: error text
 */
const char *cheats_error_text(const cheats_t *cheats)
{
	if (cheats == NULL)
		return NULL;

	return cheats->error_text;
}

/**
 * cheats_error_line - Return the line number of the last parse error.
 * @cheats: cheats
 * @return: line number
 */
int cheats_error_line(const cheats_t *cheats)
{
	if (cheats == NULL)
		return -1;

	return cheats->error_line;
}
