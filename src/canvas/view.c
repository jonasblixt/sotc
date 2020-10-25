#include <stdio.h>
#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

static double scale = 1.7;

bool sotc_region_is_root_or_offpage(struct sotc_region *r)
{
    if (r->off_page)
        return true;
    if (r->parent_state == NULL)
        return true;

    return false;
}

int sotc_get_region_absolute_coords(struct sotc_region *r, double *x,
                                                           double *y,
                                                           double *w,
                                                           double *h)
{
    struct sotc_state *ps;
    struct sotc_region *pr = r;
    double x_acc = 0.0;
    double y_acc = 0.0;

    while (pr) {
        if (sotc_region_is_root_or_offpage(pr))
            break;
        if (!pr->parent_state)
            break;
        ps = pr->parent_state;

        x_acc += ps->x;
        y_acc += ps->y + 30.0;

        pr = ps->parent_region;
    }

    *x = x_acc;
    *y = y_acc;

    if (r->parent_state) {
        *h = r->parent_state->h - 30.0;
        *w = r->parent_state->w;
    } else {
        *h = 1190;
        *w = 1684;
    }

    return 0;
}

int sotc_get_state_absolute_coords(struct sotc_state *s, double *x,
                                                         double *y,
                                                         double *w,
                                                         double *h)
{
    struct sotc_state *ps = s;
    struct sotc_region *pr;
    double x_acc = 0.0;
    double y_acc = 0.0;

    while (ps) {
      x_acc += ps->x;
      y_acc += ps->y;

      pr = ps->parent_region;

      if (sotc_region_is_root_or_offpage(pr))
        break;

      ps = pr->parent_state;

      if (ps) {
        y_acc += 30.0;
      }
    }

    *x = x_acc;
    *y = y_acc;
    *w = s->w;
    *h = s->h;
    return 0;
}

int sotc_canvas_scale(double scale_change)
{
    scale += scale_change;

    if (scale < 0.0)
        scale = 0.0;

    printf("scale = %f\n", scale);
}

double sotc_canvas_get_scale(void)
{
    return scale;
}

int sotc_canvas_render(cairo_t *cr, struct sotc_region *root,
                        int width, int height)
{
    int rc;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    static struct sotc_stack *stack;

    cairo_scale(cr, scale, scale);

    sotc_canvas_render_grid(cr, 1684, 1190);

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
