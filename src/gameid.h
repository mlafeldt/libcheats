#ifndef _GAMEID_H_
#define _GAMEID_H_

#include "mytypes.h"
#include <stddef.h> /* for size_t */

#define GID_NAME_MAX	255
#define GID_HASH_SIZE	20

#define GID_F_NONE	0
#define GID_F_NAME	1
#define GID_F_SIZE	2
#define GID_F_HASH	4
#define GID_F_ALL	(GID_F_NAME | GID_F_SIZE | GID_F_HASH)

/**
 * game_id_t - structure to hold game id information
 * @name: name of file
 * @size: size of file in bytes
 * @hash: hash of file
 * @set: GID_F_* flags showing which of the members are set
 * @tag: arbitrary information
 */
typedef struct _game_id {
	char	name[GID_NAME_MAX + 1];
	size_t	size;
	u8	hash[GID_HASH_SIZE];
	int	set;
	u32	tag;
} game_id_t;

int set_game_id(game_id_t *id, const char *name, size_t size,
			const u8 *hash, u32 tag);
int cmp_game_id(const game_id_t *id1, const game_id_t *id2);
int gen_game_id(const char *filename, game_id_t *id);

#endif /*_GAMEID_H_*/
