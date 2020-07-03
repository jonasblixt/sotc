#include <sotc/sotc.h>
#include <stdarg.h>

__attribute__ ((weak)) int sotc_debug(enum sotc_debug_level debug_level,
                                        const char *func_name,
                                        const char *fmt, ...)
{
    return 0;
}

const char * sotc_library_version(void)
{
    return PACKAGE_VERSION;
}
