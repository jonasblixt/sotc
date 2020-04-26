#ifndef INCLUDE_TCM_STACK_H_
#define INCLUDE_TCM_STACK_H_

#include <stdint.h>
#include <stddef.h>

struct tcm_stack
{
    size_t no_of_elements;
    size_t pos;
    void *data[];
};


int tcm_stack_init(struct tcm_stack **stack, size_t no_of_elements);
int tcm_stack_free(struct tcm_stack *stack);
int tcm_stack_push(struct tcm_stack *stack, void *item);
int tcm_stack_pop(struct tcm_stack *stack, void **item);

#endif  // INCLUDE_TCM_STACK_H_
