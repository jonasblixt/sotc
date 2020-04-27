#include <tcm/tcm.h>
#include <stdarg.h>

static tcm_debug_t __debug_func;

int tcm_set_debug(tcm_debug_t debug_cb)
{
    __debug_func = debug_cb;
    return TCM_OK;
}

int tcm_debug(int debug_level, const char *fmt, ...)
{
    va_list args;
    int rc;

    if (!__debug_func)
        return TCM_OK;

    va_start(args, fmt);
    rc = __debug_func(debug_level, fmt, args);
    va_end(args);
    return rc;
}

const char * tcm_library_version(void)
{
    return PACKAGE_VERSION;
}
