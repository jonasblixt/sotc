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
         do { printf("I %s: " , __func__);\
              printf(__VA_ARGS__);\
              printf("\n\r"); } while (0)

#define TCM_DEBUG(...) \
         do { printf("I %s: " , __func__);\
              printf(__VA_ARGS__);\
              printf("\n\r"); } while (0)

#define TCM_ERR(...) \
         do { fprintf(stderr, "I %s: " , __func__);\
              fprintf(stderr, __VA_ARGS__);\
              fprintf(stderr, "\n\r"); } while (0)

const char *tcm_library_version(void);

#endif  // INCLUDE_TCM_H_
