#include "Debug.hpp"
#include <stdarg.h>
#include <stdio.h>

void debug_message(const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   vprintf(fmt, ap);
   va_end(ap);
}

