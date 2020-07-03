#ifndef INCLUDE_SOTC_STACK_H_
#define INCLUDE_SOTC_STACK_H_

#include <stdint.h>
#include <stddef.h>

struct sotc_stack
{
    size_t no_of_elements;
    size_t pos;
    void *data[];
};


int sotc_stack_init(struct sotc_stack **stack, size_t no_of_elements);
int sotc_stack_free(struct sotc_stack *stack);
int sotc_stack_push(struct sotc_stack *stack, void *item);
int sotc_stack_pop(struct sotc_stack *stack, void **item);

#endif  // INCLUDE_SOTC_STACK_H_
