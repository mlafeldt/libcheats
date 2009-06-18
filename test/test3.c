/*
 * test cheats_read()
 */

#include <stdio.h>
#include "../src/libcheats.h"

int test3(int argc, char *argv[])
{
	FILE *fp;
	cheats_t cheats;

	if (argc < 2)
		return -1;

	cheats_init(&cheats);

	fp = fopen(argv[1], "r");

	if (cheats_read(&cheats, fp) != CHEATS_TRUE) {
		printf("line: %i\nerror: %s\n",
			cheats.error_line, cheats.error_text);
		fclose(fp);
		cheats_destroy(&cheats);
		return -1;
	}

	fclose(fp);
	cheats_destroy(&cheats);
	return 0;
}
