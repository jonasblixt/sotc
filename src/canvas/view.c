#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

int sotc_canvas_render(cairo_t *cr, struct sotc_region *root,
                        int width, int height)
{
    int rc;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    static struct sotc_stack *stack;

    cairo_scale(cr, 1.0, 1.0);

    sotc_canvas_render_grid(cr, width, height);

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    rc = sotc_stack_push(stack, (void *) root);

    while (sotc_stack_pop(stack, (void *) &r) == SOTC_OK)
    {
        sotc_canvas_render_region(cr, r);

        for (s = r->state; s; s = s->next)
        {
            sotc_canvas_render_state(cr, s);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

    sotc_stack_free(stack);

    return rc;
}
