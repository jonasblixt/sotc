#include <stdio.h>
#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

int transition_calc_begin_end_point(struct sotc_state *s,
                                    enum sotc_side side,
                                    double offset,
                                    double *x,
                                    double *y)
{
    switch (side) {
        case SOTC_SIDE_LEFT:
            (*x) = s->x;
            (*y) = s->y + ((offset > s->h)?s->h:offset);
        break;
        case SOTC_SIDE_RIGHT:
            (*x) = s->x + s->w;
            (*y) = s->y + ((offset > s->h)?s->h:offset);
        break;
        case SOTC_SIDE_TOP:
            (*x) = s->x + ((offset > s->w)?s->w:offset);
            (*y) = s->y;
        break;
        case SOTC_SIDE_BOTTOM:
            (*x) = s->x + ((offset > s->w)?s->w:offset);
            (*y) = s->y + s->h;
        break;
        default:
        break;
    }
}

int sotc_canvas_render_transition(cairo_t *cr,
                                  struct sotc_transition *transitions)
{
    double begin_x, begin_y, end_x, end_y;
    if (transitions == NULL)
        return SOTC_OK;

    for (struct sotc_transition *t = transitions; t; t = t->next) {

        transition_calc_begin_end_point(t->source.state,
                             t->source.side,
                             t->source.offset,
                             &begin_x, &begin_y);

        transition_calc_begin_end_point(t->dest.state,
                             t->dest.side,
                             t->dest.offset,
                             &end_x, &end_y);

        struct sotc_vertice *v;

        cairo_save(cr);
        cairo_set_source_rgb (cr, 0, 0, 0);

        if (t->focus) {
            cairo_set_source_rgb (cr, 0, 1, 0);
            cairo_save(cr);
            cairo_rectangle (cr, begin_x - 5, begin_y - 5, 10, 10);
            cairo_fill_preserve(cr);
            cairo_restore(cr);
        }

        cairo_move_to (cr, begin_x, begin_y);
        cairo_set_line_width (cr, 2.0);

        for (v = t->vertices; v; v = v->next) {
            cairo_line_to(cr, v->x, v->y);
            if (t->focus) {
                cairo_save(cr);
                cairo_rectangle (cr, v->x - 5, v->y - 5, 10, 10);
                cairo_fill_preserve(cr);
                cairo_restore(cr);
            }

            cairo_stroke (cr);

            cairo_move_to (cr, v->x, v->y);
            begin_x = v->x;
            begin_y = v->y;
        }

        cairo_line_to(cr, end_x, end_y);

        if (t->focus) {
            cairo_set_source_rgb (cr, 0, 1, 0);
            cairo_save(cr);
            cairo_rectangle (cr, end_x - 5, end_y - 5, 10, 10);
            cairo_fill_preserve(cr);
            cairo_restore(cr);
        } else {
            cairo_set_source_rgb (cr, 0, 0, 0);
        }
        cairo_set_line_width (cr, 2.0);
        cairo_stroke (cr);
        cairo_restore(cr);

        /* Draw arrow */
        double angle = atan2 (end_y - begin_y, end_x - begin_x) + M_PI;

        double x1 = end_x + 15 * cos(angle - 0.4);
        double y1 = end_y + 15 * sin(angle - 0.4);
        double x2 = end_x + 15 * cos(angle + 0.4);
        double y2 = end_y + 15 * sin(angle + 0.4);

        cairo_save(cr);

        if (t->focus)
            cairo_set_source_rgb (cr, 0, 1, 0);
        else
            cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_new_sub_path(cr);
        cairo_move_to (cr, end_x, end_y);
        cairo_line_to(cr, x1, y1);
        cairo_line_to(cr, x2, y2);
        cairo_close_path(cr);
        cairo_fill(cr);
        cairo_restore(cr);

        /* Draw text box */
        char text[1024];
        cairo_save(cr);
        cairo_set_font_size (cr, 18);
        cairo_set_source_rgb (cr, 0,0,0);
        cairo_move_to (cr, t->text_block_coords.x,
                           t->text_block_coords.y);
        snprintf(text, sizeof(text), "%s [%s] / %s",
                    t->trigger->name, "", "");
        cairo_show_text (cr, text);
        cairo_restore(cr);

    }

    return SOTC_OK;
}
