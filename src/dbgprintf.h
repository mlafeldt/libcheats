#ifndef _DBGPRINTF_H_
#define _DBGPRINTF_H_

#include <stdio.h>

#ifdef _DEBUG
	#define D_PRINTF(args...)	printf(args)
#else
	#define D_PRINTF(args...)	do {} while (0)
#endif

#endif /* _DBGPRINTF_H_ */
