#include <stdio.h>
#include "mytypes.h"
#include "readcheats.h"

int main(int argc, char *argv[])
{
	gamelist_t list;

	cl_init(&list);

	rc_read_file(&list, argv[1]);

	cl_print(&list);
	cl_free(&list);

	return 0;
}
