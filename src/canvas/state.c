#include <stdio.h>
#include <string.h>
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

    double y_offset = 0.0;

    if (state->entries || state->exits) {
        y_offset = 50.0;
        cairo_move_to (cr, x, y + 30);
        cairo_line_to(cr, x + w, y + 30);
    } else {
        y_offset = 30;
    }

    /* Render entry actions */
    struct sotc_action_ref *entry;
    sotc_state_get_entries(state, &entry);
    char action_str_buf[128];

    for (;entry; entry = entry->next) {
        snprintf(action_str_buf, sizeof(action_str_buf),
                    "entry/ %s", entry->act->name);
        cairo_move_to(cr, x + 10, y + y_offset);
        cairo_show_text(cr, action_str_buf);
        y_offset += 20;
    }

    sotc_state_get_exits(state, &entry);
    for (;entry; entry = entry->next) {
        snprintf(action_str_buf, sizeof(action_str_buf),
                    "exit/ %s", entry->act->name);
        cairo_move_to(cr, x + 10, y + y_offset);
        cairo_show_text(cr, action_str_buf);
        y_offset += 20;
    }

    if (state->regions) {
        cairo_move_to (cr, x, y + y_offset);
        cairo_line_to(cr, x + w, y + y_offset);
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

    state->region_y_offset = y_offset - 30.0;
    return 0;
}
