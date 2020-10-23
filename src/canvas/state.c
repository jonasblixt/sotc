#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"


int sotc_canvas_render_state(cairo_t *cr, struct sotc_state *state)
{
    double lbl_x, lbl_y;
    double radius = 10.0;
    double degrees = M_PI / 180.0;
    cairo_text_extents_t extents;

    cairo_save(cr);
    cairo_new_sub_path(cr);
    cairo_arc(cr, state->x + state->w - radius,
                  state->y + radius,
                  radius, -90 * degrees, 0);

    cairo_arc(cr, state->x + state->w - radius,
                  state->y + state->h - radius,
                  radius, 0 * degrees, 90 * degrees);

    cairo_arc(cr, state->x + radius,
                  state->y + state->h - radius,
                  radius, 90 * degrees, 180 * degrees);

    cairo_arc(cr, state->x + radius,
                  state->y + radius, radius,
                  180 * degrees, 270 * degrees);

    cairo_close_path(cr);
    cairo_fill_preserve(cr);

    cairo_move_to (cr, state->x, state->y + 30);

    cairo_line_to(cr, state->x + state->w, state->y + 30);
    cairo_set_font_size (cr, 18);
    cairo_set_source_rgb (cr, 0,0,0);

    cairo_text_extents (cr, state->name, &extents);

    lbl_x = (state->x + state->w/2.0) - (extents.width/2 + extents.x_bearing);
    lbl_y = (state->y + 15) - (extents.height/2 + extents.y_bearing);

    cairo_move_to (cr, lbl_x, lbl_y);
    cairo_show_text (cr, state->name);

    if (state->focus)
        cairo_set_source_rgb (cr, 0, 1, 0);
    else
        cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);
    cairo_restore (cr);

    return 0;
}
