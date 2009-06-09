#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameid.h"

/**
 * set_game_id - Sets up the members of a game id.
 * @id: ptr to game id
 * @name: filename
 * @size: file size (greater than 0)
 * @hash: hash of file
 * @tag: arbitrary information
 * @return: GID_F_* flags showing which of the members were set
 */
int set_game_id(game_id_t *id, const char *name, size_t size,
			const u8 *hash, u32 tag)
{
	return GID_F_NONE;
}

/**
 * cmp_game_id - Compares two game ids.
 * @id1: ptr to 1st game id
 * @id2: ptr to 2nd game id
 * @return: GID_F_* flags showing which of the members are equal
 */
int cmp_game_id(const game_id_t *id1, const game_id_t *id2)
{
	int ret = GID_F_NONE;

	return ret;
}

/**
 * gen_game_id - Generates the game id from a file.
 * @filename: full path to the file
 * @id: ptr to where the game id will be written to
 * @return: 0: success, <0: error
 */
int gen_game_id(const char *filename, game_id_t *id)
{
	return 0;
}
