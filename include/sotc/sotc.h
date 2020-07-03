#ifndef INCLUDE_SOTC_H_
#define INCLUDE_SOTC_H_

#include <stdarg.h>

/* This sets the upper limit on how many regions a state can hold
 * and how many states a region can hold */
#define SOTC_MAX_R_S 1024

/* This sets the total amount of states and regions that can be 
 * allocated */
#define SOTC_MAX_OBJECTS (1024*1024)

enum sotc_errors
{
    SOTC_OK,
    SOTC_ERROR,
    SOTC_ERR_IO,
    SOTC_ERR_MEM,
    SOTC_ERR_PARSE,
};

enum sotc_debug_level
{
    SOTC_L_ERROR,
    SOTC_L_INFO,
    SOTC_L_DEBUG,
};

#define L_INFO(...) \
         do { sotc_debug(1, __func__, __VA_ARGS__); } while (0)

#define L_DEBUG(...) \
         do { sotc_debug(2, __func__, __VA_ARGS__); } while (0)

#define L_ERR(...) \
         do { sotc_debug(0, __func__, __VA_ARGS__); } while (0)

int sotc_debug(enum sotc_debug_level debug_level, const char *func_name,
                    const char *fmt, ...);

const char *sotc_library_version(void);

#endif  // INCLUDE_SOTC_H_
