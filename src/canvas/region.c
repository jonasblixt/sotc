#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

int sotc_canvas_render_region(cairo_t *cr, struct sotc_region *region)
{
    double x, y, w, h;
    double dashes[] = {10.0,  /* ink */
                       10.0};  /* skip */

    double y_offset = 0.0;

    if (!region->focus)
        return 0;

    if (region->parent_state)
        y_offset = region->parent_state->region_y_offset;

    sotc_get_region_absolute_coords(region, &x, &y, &w, &h);

    cairo_save (cr);
    cairo_set_font_size (cr, 18);
    cairo_set_source_rgb (cr, 0,0,0);
    cairo_move_to (cr, x + 10, y + y_offset + 25);
    cairo_show_text (cr, region->name);

    cairo_set_source_rgb (cr, 0.9, 0.9, 0.9);
    cairo_set_dash (cr, dashes, 2, 0);
    cairo_set_line_width (cr, 2);
    cairo_rectangle (cr, x, y + y_offset, w, h - y_offset);
    cairo_stroke (cr);
    cairo_restore (cr);

    return 0;
}
