/*
 * test cheats_read()
 */

#include <stdio.h>
#include "libcheats.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	cheats_t cheats;

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
