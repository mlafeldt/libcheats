#include <stdio.h>
#include "mytypes.h"
#include "readcheats.h"

int main(int argc, char *argv[])
{
	cheats_t cheats;

	cheats_init(&cheats);
	cheats_read_file(&cheats, argv[1]);
	cheats_write_file(&cheats, "out");

	cl_print(&cheats.games);
	cheats_destroy(&cheats);

	return 0;
}
