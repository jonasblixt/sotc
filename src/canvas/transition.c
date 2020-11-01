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
    double sx, sy, sw, sh;

    sotc_get_state_absolute_coords(s, &sx, &sy, &sw, &sh);

    if (s->kind == SOTC_STATE_NORMAL) {
        switch (side) {
            case SOTC_SIDE_LEFT:
                (*x) = sx;
                (*y) = sy + ((offset > sh)?sh:offset);
            break;
            case SOTC_SIDE_RIGHT:
                (*x) = sx + sw;
                (*y) = sy + ((offset > sh)?sh:offset);
            break;
            case SOTC_SIDE_TOP:
                (*x) = sx + ((offset > sw)?sw:offset);
                (*y) = sy;
            break;
            case SOTC_SIDE_BOTTOM:
                (*x) = sx + ((offset > sw)?sw:offset);
                (*y) = sy + sh;
            break;
            default:
            break;
        }
    } else if (s->kind == SOTC_STATE_INIT) {
        (*x) = sx + sw/2;
        (*y) = sy + sh;
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

        double rx, ry, rw, rh;

        sotc_get_region_absolute_coords(t->source.state->parent_region,
                                        &rx, &ry, &rw, &rh);

        cairo_save(cr);
        cairo_move_to (cr, begin_x, begin_y);
        cairo_set_line_width (cr, 2.0);

        for (v = t->vertices; v; v = v->next) {
            cairo_line_to(cr, v->x, v->y);
            //cairo_set_source_rgb (cr, 0, 0, 0);
            sotc_color_set(cr, SOTC_COLOR_NORMAL);
            cairo_stroke (cr);
            cairo_move_to (cr, v->x, v->y);
            begin_x = v->x;
            begin_y = v->y;
        }

        cairo_line_to(cr, end_x, end_y);
        //cairo_set_source_rgb (cr, 0, 0, 0);
        sotc_color_set(cr, SOTC_COLOR_NORMAL);
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

        //cairo_set_source_rgb (cr, 0, 0, 0);
        sotc_color_set(cr, SOTC_COLOR_NORMAL);
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
        //cairo_set_source_rgb (cr, 0,0,0);
        sotc_color_set(cr, SOTC_COLOR_NORMAL);
        cairo_move_to (cr, rx + t->text_block_coords.x,
                           ry + t->text_block_coords.y);

        enum sotc_state_kind source_kind = t->source.state->kind;

        if (source_kind == SOTC_STATE_NORMAL) {
            snprintf(text, sizeof(text), "%s [%s] / %s",
                        t->trigger->name, "", "");
        } else if (source_kind == SOTC_STATE_INIT) {
            snprintf(text, sizeof(text), "/ %s", "");
        }

        cairo_show_text (cr, text);
        cairo_restore(cr);

        /* Draw selection boxes if focused */

        if (t->focus) {
            double fbx, fby;

            transition_calc_begin_end_point(t->source.state,
                                 t->source.side,
                                 t->source.offset,
                                 &fbx, &fby);

            cairo_save(cr);
            sotc_color_set(cr, SOTC_COLOR_ACCENT);
            cairo_rectangle (cr, fbx - 5, fby - 5, 10, 10);
            cairo_fill(cr);

            cairo_move_to (cr, fbx, fby);

            for (v = t->vertices; v; v = v->next) {
                cairo_rectangle (cr, v->x - 5, v->y - 5, 10, 10);
                cairo_fill(cr);
                cairo_move_to (cr, v->x, v->y);
                fbx = v->x;
                fby = v->y;
            }

            cairo_rectangle (cr, end_x - 5, end_y - 5, 10, 10);
            cairo_fill(cr);
            cairo_restore(cr);
        }

    }

    return SOTC_OK;
}
