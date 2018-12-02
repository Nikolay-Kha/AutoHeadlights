#ifndef DEBUG_H_
#define DEBUG_H_

#include "config.h"

#ifdef DEBUG

#include <stdarg.h>
void debug_init();
void printf(const char *pFormat, ...);
void debug(const char *pFormat, ...);

#else

#define debug_init()
#define printf(format, ...)
#define debug(format, ...)

#endif // DEBUG


#endif /* DEBUG_H_ */
