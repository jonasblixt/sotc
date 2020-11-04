#include <stdio.h>
#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

static double scale = 1.7;
static double selection_sx;
static double selection_sy;
static double selection_ex;
static double selection_ey;
static bool draw_selection;
static double pan_x, pan_y;

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

    if (r->draw_as_root) {
        *x = 0;
        *y = 0;
        *h = 1190;
        *w = 1684;
        return SOTC_OK;
    }

    /* Calculate offsets to current state */
    while (pr) {
        if (!pr->parent_state)
            break;
        ps = pr->parent_state;

        x_acc += ps->x;
        y_acc += ps->y + 30.0;

        pr = ps->parent_region;
    }

    /* Iterate through possible sibling regions */
    ps = r->parent_state;

    if (ps) {
        pr = ps->regions;

        while (pr) {
            if (pr == r)
                break;
            y_acc += pr->h;
            pr = pr->next;
        }
    }


    *x = x_acc;
    *y = y_acc;

    if (r->h == -1) {
        *h = r->parent_state->h - 30.0;
    } else {
        *h = r->h;
    }
    *w = r->parent_state->w;

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

    if (scale < 0.1)
        scale = 0.1;

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
    double x, y, w, h;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    static struct sotc_stack *stack;

    cairo_translate(cr, pan_x, pan_y);
    cairo_scale(cr, scale, scale);

    sotc_canvas_render_grid(cr, 1684, 1190);

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    /* Pass 1: draw states, regions etc*/
    rc = sotc_stack_push(stack, (void *) root);

    while (sotc_stack_pop(stack, (void *) &r) == SOTC_OK)
    {
        sotc_canvas_render_region(cr, r);

        if (r->off_page && (!r->draw_as_root))
            continue;

        for (s = r->state; s; s = s->next)
        {
            sotc_canvas_render_state(cr, s);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

    /* Pass 2: draw transitions */
    rc = sotc_stack_push(stack, (void *) root);

    while (sotc_stack_pop(stack, (void *) &r) == SOTC_OK)
    {
        for (s = r->state; s; s = s->next)
        {
            sotc_canvas_render_transition(cr, s->transition);
            for (r2 = s->regions; r2; r2 = r2->next)
            {
                if (r2->off_page)
                    continue;

                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

    sotc_stack_free(stack);

    /* Draw selection overlay */

    double dashes[] = {10.0,  /* ink */
                       20.0};  /* skip */

    if (draw_selection) {
        cairo_save(cr);
        //cairo_set_source_rgb (cr, 0.4, 0.4, 0.4);
        sotc_color_set(cr, SOTC_COLOR_ACCENT);
        cairo_set_dash (cr, dashes, 2, 0);
        cairo_set_line_width (cr, 1);
        cairo_rectangle (cr, selection_sx,
                             selection_sy,
                             selection_ex - selection_sx,
                             selection_ey - selection_sy);
        cairo_stroke (cr);
        cairo_restore (cr);
    }
    return rc;
}

int sotc_canvas_set_selection(bool active, double sx,
                                           double sy,
                                           double ex,
                                           double ey)
{
    draw_selection = active;
    selection_sx = sx;
    selection_sy = sy;
    selection_ex = ex;
    selection_ey = ey;
}

int sotc_canvas_pan(double dx, double dy)
{
    pan_x += dx;
    pan_y += dy;
    L_DEBUG("pan <%f, %f>", pan_x, pan_y);
}

int sotc_canvas_get_offset(double *x, double *y)
{
    (*x) = pan_x;
    (*y) = pan_y;
}

double distance_point_to_seg2(double px, double py,
                            double sx, double sy,
                            double ex, double ey,
                            double *x_out, double *y_out)
{
    double A = px - sx;
    double B = py - sy;
    double C = ex - sx;
    double D = ey - sy;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;

    if (len_sq != 0) //in case of 0 length line
        param = dot / len_sq;

    double xx, yy;

    if (param < 0) {
        xx = sx;
        yy = sy;
    } else if (param > 1) {
        xx = ex;
        yy = ey;
    } else {
        xx = sx + param * C;
        yy = sy + param * D;
    }

    double dx = px - xx;
    double dy = py - yy;
/*
    printf("distance from <%f, %f> to line <<%f, %f>, <%f, %f>>\n",
            px, py, sx, sy, ex, ey);
*/

    if (x_out)
        *x_out = xx;
    if (y_out)
        *y_out = yy;

    return sqrt(dx * dx + dy * dy);
}


double distance_point_to_seg(double px, double py,
                            double sx, double sy,
                            double ex, double ey)
{
    return distance_point_to_seg2(px, py, sx, sy, ex, ey, NULL, NULL);
}
