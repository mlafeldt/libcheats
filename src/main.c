#include <stdio.h>
#include "mytypes.h"
#include "libcheats.h"

int main(int argc, char *argv[])
{
	cheats_t cheats;

	cheats_init(&cheats);

	if (cheats_read_file(&cheats, argv[1]) != CHEATS_TRUE) {
		printf("%s:%i: error: %s\n", cheats.source, cheats.error_line, cheats.error_text);
	}

	cheats_write_file(&cheats, "out");
//	cheats_write(&cheats, stdout);
	cheats_destroy(&cheats);

	return 0;
}
