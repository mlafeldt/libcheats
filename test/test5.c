/*
 * test TAILQ_* macros
 */

#include <stdio.h>
#include <libcheats.h>

void add_game(gamelist_t *list)
{
	game_t *game;
	cheat_t *cheat;
	code_t *code;

	game = make_game("Foo Game", NULL, 0);
	TAILQ_INSERT_TAIL(list, game, node);

	cheat = make_cheat("Inf. Bar", NULL, 0);
	TAILQ_INSERT_TAIL(&game->cheats, cheat, node);

	code = make_code(0x123, 0x456, 0);
	TAILQ_INSERT_TAIL(&cheat->codes, code, node);
	code = make_code(0x789, 0, 0);
	TAILQ_INSERT_TAIL(&cheat->codes, code, node);
}

int test5(int argc, char *argv[])
{
	cheats_t cheats;

	cheats_init(&cheats);
	add_game(&cheats.games);
	cheats_destroy(&cheats);

	return 0;
}
