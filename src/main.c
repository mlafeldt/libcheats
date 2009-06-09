#include <stdio.h>
#include "mytypes.h"
#include "readcheats.h"

int main(int argc, char *argv[])
{
	gamelist_t list;

	cl_init(&list);

	rc_from_textfile(argv[1], &list, 0);

	cl_print(&list);
	cl_free(&list);

	return 0;
}
