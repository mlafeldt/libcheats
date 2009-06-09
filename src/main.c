#include <stdio.h>
#include "mytypes.h"
#include "codeman.h"

int main(int argc, char *argv[])
{
	gamelist_t list;

	cl_init(&list);

	codes_from_textfile(argv[1], &list, CODES_OPT_GID);

	cl_print(&list);
	cl_free(&list);

	return 0;
}
