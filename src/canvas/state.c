#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/view.h"


static int render_init_state(cairo_t *cr, struct sotc_state *state)
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
    cairo_set_line_width(cr, 4);
    if (state->focus)
        sotc_color_set(cr, SOTC_COLOR_ACCENT);
    else
        sotc_color_set(cr, SOTC_COLOR_NORMAL);
    cairo_translate(cr, x+w/2, y+h/2);
    cairo_arc(cr, 0, 0, w/2, 0, 2 * M_PI);
    cairo_stroke_preserve(cr);
    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill_preserve(cr);
    sotc_color_set(cr, SOTC_COLOR_BG);
    cairo_fill(cr);
    cairo_restore(cr);
/*
    cairo_save(cr);
    cairo_rectangle(cr, x, y, w, h);
    sotc_color_set(cr, SOTC_COLOR_ACCENT);
    cairo_stroke(cr);
    cairo_restore(cr);
*/
    return 0;
}

static int render_normal_state(cairo_t *cr, struct sotc_state *state)
{
    double x, y, w, h;
    double rx, ry, rw, rh;
    double lbl_x, lbl_y;
    double radius = 10.0;
    double degrees = M_PI / 180.0;
    bool clip_text = false;
    cairo_text_extents_t extents;

    sotc_get_state_absolute_coords(state, &x, &y, &w, &h);

    //cairo_set_source_rgb (cr, 1, 1, 1);
    sotc_color_set(cr, SOTC_COLOR_BG);
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

    //cairo_set_source_rgb (cr, 0,0,0);
    sotc_color_set(cr, SOTC_COLOR_NORMAL);

    cairo_set_font_size (cr, 18);
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

    cairo_save(cr);
    cairo_set_font_size (cr, 14);
    for (;entry; entry = entry->next) {
        snprintf(action_str_buf, sizeof(action_str_buf),
                    "e/ %s()", entry->act->name);
        cairo_move_to(cr, x + 10, y + y_offset);
        cairo_show_text(cr, action_str_buf);
        y_offset += 20;
    }

    sotc_state_get_exits(state, &entry);
    for (;entry; entry = entry->next) {
        snprintf(action_str_buf, sizeof(action_str_buf),
                    "x/ %s()", entry->act->name);
        cairo_move_to(cr, x + 10, y + y_offset);
        cairo_show_text(cr, action_str_buf);
        y_offset += 20;
    }

    if (state->regions) {
        cairo_move_to (cr, x, y + y_offset);
        cairo_line_to(cr, x + w, y + y_offset);
    }

    cairo_restore(cr);
    //cairo_set_source_rgb (cr, 0, 0, 0);
    sotc_color_set(cr, SOTC_COLOR_NORMAL);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);
    cairo_restore (cr);

    if (clip_text) {
        cairo_save(cr);
        cairo_rectangle(cr, x+2, y+2, w-4, 28);
        cairo_clip(cr);
        cairo_move_to (cr, lbl_x, lbl_y);
        //cairo_set_source_rgb(cr, 0, 0, 0);
        sotc_color_set(cr, SOTC_COLOR_NORMAL);
        cairo_set_font_size (cr, 18);
        cairo_show_text (cr, state->name);
        cairo_restore(cr);
    }

    if (state->focus) {
        cairo_save(cr);
        sotc_color_set(cr, SOTC_COLOR_ACCENT);
        cairo_rectangle (cr, (x + w/2) - 5, y - 5, 10, 10);     /* Top */
        cairo_rectangle (cr, (x + w/2) - 5, y + h - 5, 10, 10); /* Bot */
        cairo_rectangle (cr, x - 5, (y + h/2) - 5, 10, 10);     /* Left */
        cairo_rectangle (cr, x + w - 5, (y + h/2) - 5, 10, 10);     /* Right */
        /* Corners */
        cairo_rectangle (cr, x - 5, y - 5, 10, 10);
        cairo_rectangle (cr, x + w - 5, y - 5, 10, 10);
        cairo_rectangle (cr, x + w - 5, y + h - 5, 10, 10);
        cairo_rectangle (cr, x - 5, y + h - 5, 10, 10);
        cairo_fill(cr);
        cairo_restore(cr);

    }


    state->region_y_offset = y_offset - 30.0;
}

int sotc_canvas_render_state(cairo_t *cr, struct sotc_state *state)
{
    int rc;

    switch (state->kind) {
        case SOTC_STATE_NORMAL:
            rc = render_normal_state(cr, state);
        break;
        case SOTC_STATE_INIT:
            rc = render_init_state(cr, state);
        break;
    }
    return 0;
}

int sotc_canvas_state_translate(struct sotc_state *s, double dx, double dy)
{
    s->x += dx;
    s->y += dy;

    for (struct sotc_transition *t = s->transition; t; t = t->next) {
        if ((t->source.state == s) &&
            (t->dest.state == s)) {

            t->text_block_coords.x += dx;
            t->text_block_coords.y += dy;

            for (struct sotc_vertice *v = t->vertices; v; v = v->next) {
                v->x += dx;
                v->y += dy;
            }
        }
    }
}

int sotc_state_get_at_xy(struct sotc_region *region, double px, double py,
                            struct sotc_state **out, int *depth)
{
    int d = 0;
    static struct sotc_stack *stack;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    bool found_state = false;
    double x, y, w, h;
    double ox, oy;

    sotc_canvas_get_offset(&ox, &oy);

    ox = ox / sotc_canvas_get_scale();
    oy = oy / sotc_canvas_get_scale();

    sotc_stack_init(&stack, SOTC_MAX_R_S);
    sotc_stack_push(stack, region);

    while (sotc_stack_pop(stack, (void **) &r) == SOTC_OK)
    {
        if (r->off_page && !r->draw_as_root)
            continue;
        d++;

        for (s = r->state; s; s = s->next)
        {
            sotc_get_state_absolute_coords(s, &x, &y, &w, &h);

            x += ox;
            y += oy;

            if ( (px > (x-5)) && (px < (x + w + 5)) &&
                 (py > (y-5)) && (py < (y + h + 5))) {

                 L_DEBUG("State '%s' selected", s->name);
                 (*out) = s;
                 found_state = true;
            }
            for (r2 = s->regions; r2; r2 = r2->next)
            {
                sotc_stack_push(stack, r2);
            }
        }
    }

    if (depth != NULL)
        (*depth) = d;

    if (found_state)
        return SOTC_OK;
    else
        return -SOTC_ERROR;
}

int sotc_state_get_closest_side(struct sotc_state *s, double px, double py,
                                    enum sotc_side *side, double *offset)
{
    double x, y, w, h;
    double ox, oy;
    double d, d2;
    double lx, ly;

    sotc_get_state_absolute_coords(s, &x, &y, &w, &h);
    sotc_canvas_get_offset(&ox, &oy);

    ox = ox / sotc_canvas_get_scale();
    oy = oy / sotc_canvas_get_scale();

    x += ox;
    y += oy;

    /* Top segment */
    d = fabs(distance_point_to_seg2(px, py, x, y, x + w, y, &lx, &ly));
    *side = SOTC_SIDE_TOP;
    *offset = lx - x;

    /* Right segment */
    d2 = fabs(distance_point_to_seg2(px, py, x + w, y, x + w, y + h, &lx, &ly));

    if (d2 < d) {
        d = d2;
        *side = SOTC_SIDE_RIGHT;
        *offset = ly - y;
    }

    /* Bottom segment */
    d2 = fabs(distance_point_to_seg2(px, py, x, y + h, x + w, y + h, &lx, &ly));

    if (d2 < d) {
        d = d2;
        *side = SOTC_SIDE_BOTTOM;
        *offset = lx - x;
    }

    /* Left segment */
    d2 = fabs(distance_point_to_seg2(px, py, x, y, x, y + h, &lx, &ly));

    if (d2 < d) {
        d = d2;
        *side = SOTC_SIDE_LEFT;
        *offset = ly - y;
    }

    return SOTC_OK;
}
