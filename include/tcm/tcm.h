#ifndef INCLUDE_TCM_H_
#define INCLUDE_TCM_H_

enum tcm_errors
{
    TCM_OK,
    TCM_ERROR,
    TCM_ERR_IO,
    TCM_ERR_MEM,
};

#define TCM_INFO(...) \
         do { tcm_debug(1, __VA_ARGS__); } while (0)

#define TCM_DEBUG(...) \
         do { tcm_debug(2, __VA_ARGS__); } while (0)

#define TCM_ERR(...) \
         do { tcm_debug(0, __VA_ARGS__); } while (0)

typedef int (*tcm_debug_t)(int debug_level, const char *fmt, ...);

int tcm_debug(int debug_level, const char *fmt, ...);
const char *tcm_library_version(void);
int tcm_set_debug(tcm_debug_t debug_cb);

#endif  // INCLUDE_TCM_H_
