#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tcm/tcm.h>
#include <tcm/stack.h>

int tcm_stack_init(struct tcm_stack **stack_pp, size_t no_of_elements)
{
    struct tcm_stack *stack = NULL;
    size_t bytes_to_alloc = sizeof(struct tcm_stack) +
                    (sizeof(void *) * no_of_elements);

    stack = malloc(bytes_to_alloc);

    if (!stack)
    {
        L_ERR("Could not allocate memory for stack");
        return -TCM_ERR_MEM;
    }

    memset(stack, 0, bytes_to_alloc);
    (*stack_pp) = stack;

    stack->no_of_elements = no_of_elements;
    stack->pos = 0;

    return TCM_OK;
}

int tcm_stack_free(struct tcm_stack *stack)
{
    free(stack);
    return TCM_OK;
}

int tcm_stack_push(struct tcm_stack *stack, void *item)
{
    if (stack->pos >= stack->no_of_elements)
        return -TCM_ERROR;

    stack->data[stack->pos++] = item;

    return TCM_OK;
}

int tcm_stack_pop(struct tcm_stack *stack, void **item)
{
    if(!stack->pos)
        return -TCM_ERROR;

    (*item) = stack->data[--stack->pos];

    return TCM_OK;
}
