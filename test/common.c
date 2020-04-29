#include <stdio.h>
#include <tcm/tcm.h>
#include <stdarg.h>
#include "common.h"

int tcm_debug(enum tcm_debug_level debug_level, const char *fmt, ...)
{
    va_list args;
    int rc;

    va_start(args, fmt);
    switch (debug_level)
    {
        case 0:
            printf("E ");
        break;
        case 1:
            printf("I ");
        break;
        case 2:
            printf("D ");
        break;
    }
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    return rc;
}
