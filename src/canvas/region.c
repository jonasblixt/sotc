#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"

int sotc_canvas_render_region(cairo_t *cr, struct sotc_region *region)
{
    double dashes[] = {10.0,  /* ink */
                       10.0};  /* skip */

    cairo_save (cr);

    cairo_set_font_size (cr, 18);
    cairo_set_source_rgb (cr, 0,0,0);
    cairo_move_to (cr, region->x + 10, region->y + 15);
    cairo_show_text (cr, region->name);

    if (region->focus)
        cairo_set_source_rgb (cr, 0,1,0);
    else
        cairo_set_source_rgb (cr, 1,0,0);

    cairo_set_dash (cr, dashes, 2, 0);
    cairo_set_line_width (cr, 2);
    cairo_rectangle (cr, region->x, region->y, region->w, region->h);
    cairo_stroke (cr);
    cairo_restore (cr);
}
