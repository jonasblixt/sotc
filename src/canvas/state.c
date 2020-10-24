#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

int sotc_canvas_render_state(cairo_t *cr, struct sotc_state *state)
{
    double x, y, w, h;
    double lbl_x, lbl_y;
    double radius = 10.0;
    double degrees = M_PI / 180.0;
    bool clip_text = false;
    cairo_text_extents_t extents;

    sotc_get_state_absolute_coords(state, &x, &y, &w, &h);

    cairo_save(cr);
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - radius,
                  y + radius,
                  radius, -90 * degrees, 0);

    cairo_arc(cr, x + w - radius,
                  y + h - radius,
                  radius, 0 * degrees, 90 * degrees);

    cairo_arc(cr, x + radius,
                  y + h - radius,
                  radius, 90 * degrees, 180 * degrees);

    cairo_arc(cr, x + radius,
                  y + radius, radius,
                  180 * degrees, 270 * degrees);

    cairo_close_path(cr);
    cairo_fill_preserve(cr);

    cairo_move_to (cr, x, y + 30);
    if (state->regions)
        cairo_line_to(cr, x + w, y + 30);
    cairo_set_font_size (cr, 18);
    cairo_set_source_rgb (cr, 0,0,0);

    cairo_text_extents (cr, state->name, &extents);

    if (extents.width < state->w) {
        lbl_x = (x + w/2.0) - (extents.width/2 + extents.x_bearing);
    } else {
        /* Text extends beyond the header area, left adjust and clip*/
        lbl_x = (x + 10);
        clip_text = true;
    }

    lbl_y = (y + 15) - (extents.height/2 + extents.y_bearing);

    if (!clip_text) {
        cairo_move_to (cr, lbl_x, lbl_y);
        cairo_show_text (cr, state->name);
    }

    if (state->focus)
        cairo_set_source_rgb (cr, 0, 1, 0);
    else
        cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);
    cairo_restore (cr);

    if (clip_text) {
        cairo_save(cr);
        cairo_rectangle(cr, x+2, y+2, w-4, 28);
        cairo_clip(cr);
        cairo_move_to (cr, lbl_x, lbl_y);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_font_size (cr, 18);
        cairo_show_text (cr, state->name);
        cairo_restore(cr);
    }

    return 0;
}
