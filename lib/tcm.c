#include <tcm/tcm.h>
#include <stdarg.h>

__attribute__ ((weak)) int tcm_debug(enum tcm_debug_level debug_level,
                                        const char *func_name,
                                        const char *fmt, ...)
{
    return 0;
}

const char * tcm_library_version(void)
{
    return PACKAGE_VERSION;
}
