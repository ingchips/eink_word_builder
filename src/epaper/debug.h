#ifndef __DEBUG_H
#define __DEBUG_H

#include "platform_api.h"

#define DEBUG 1
#if DEBUG
	#define Debug platform_printf
#else
	#define Debug(__info,...)
#endif

#endif