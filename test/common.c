#include <stdio.h>
#include <tcm/tcm.h>
#include <stdarg.h>
#include "common.h"

int tcm_test_debug(int debug_level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    return 0;
}
