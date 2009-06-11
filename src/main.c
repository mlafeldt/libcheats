#include <stdio.h>
#include "mytypes.h"
#include "libcheats.h"

int main(int argc, char *argv[])
{
	cheats_t cheats;

	cheats_init(&cheats);
	cheats_read_file(&cheats, argv[1]);
	cheats_write_file(&cheats, "out");
//	cheats_write(&cheats, stdout);
	cheats_destroy(&cheats);

	return 0;
}
