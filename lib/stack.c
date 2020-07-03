#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sotc/sotc.h>
#include <sotc/stack.h>

int sotc_stack_init(struct sotc_stack **stack_pp, size_t no_of_elements)
{
    struct sotc_stack *stack = NULL;
    size_t bytes_to_alloc = sizeof(struct sotc_stack) +
                    (sizeof(void *) * no_of_elements);

    stack = malloc(bytes_to_alloc);

    if (!stack)
    {
        L_ERR("Could not allocate memory for stack");
        return -SOTC_ERR_MEM;
    }

    memset(stack, 0, bytes_to_alloc);
    (*stack_pp) = stack;

    stack->no_of_elements = no_of_elements;
    stack->pos = 0;

    return SOTC_OK;
}

int sotc_stack_free(struct sotc_stack *stack)
{
    free(stack);
    return SOTC_OK;
}

int sotc_stack_push(struct sotc_stack *stack, void *item)
{
    if (stack->pos >= stack->no_of_elements)
        return -SOTC_ERROR;

    stack->data[stack->pos++] = item;

    return SOTC_OK;
}

int sotc_stack_pop(struct sotc_stack *stack, void **item)
{
    if(!stack->pos)
        return -SOTC_ERROR;

    (*item) = stack->data[--stack->pos];

    return SOTC_OK;
}
