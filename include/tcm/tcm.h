#ifndef INCLUDE_TCM_H_
#define INCLUDE_TCM_H_

#include <stdarg.h>

enum tcm_errors
{
    TCM_OK,
    TCM_ERROR,
    TCM_ERR_IO,
    TCM_ERR_MEM,
};

enum tcm_debug_level
{
    TCM_L_ERROR,
    TCM_L_INFO,
    TCM_L_DEBUG,
};

#define L_INFO(...) \
         do { tcm_debug(1, __VA_ARGS__); } while (0)

#define L_DEBUG(...) \
         do { tcm_debug(2, __VA_ARGS__); } while (0)

#define L_ERR(...) \
         do { tcm_debug(0, __VA_ARGS__); } while (0)

typedef int (*tcm_debug_t)(enum tcm_debug_level debug_level, const char *fmt,
                            va_list args);

int tcm_debug(enum tcm_debug_level debug_level, const char *fmt, ...);
const char *tcm_library_version(void);

#endif  // INCLUDE_TCM_H_
