#ifndef INCLUDE_TCM_H_
#define INCLUDE_TCM_H_

#include <stdarg.h>

/* This sets the upper limit on how many regions a state can hold
 * and how many states a region can hold */
#define TCM_MAX_R_S 1024

/* This sets the total amount of states and regions that can be 
 * allocated */
#define TCM_MAX_OBJECTS (1024*1024)

enum tcm_errors
{
    TCM_OK,
    TCM_ERROR,
    TCM_ERR_IO,
    TCM_ERR_MEM,
    TCM_ERR_PARSE,
};

enum tcm_debug_level
{
    TCM_L_ERROR,
    TCM_L_INFO,
    TCM_L_DEBUG,
};

#define L_INFO(...) \
         do { tcm_debug(1, __func__, __VA_ARGS__); } while (0)

#define L_DEBUG(...) \
         do { tcm_debug(2, __func__, __VA_ARGS__); } while (0)

#define L_ERR(...) \
         do { tcm_debug(0, __func__, __VA_ARGS__); } while (0)

int tcm_debug(enum tcm_debug_level debug_level, const char *func_name,
                    const char *fmt, ...);

const char *tcm_library_version(void);

#endif  // INCLUDE_TCM_H_
