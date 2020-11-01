#ifndef CANVAS_VIEW_H_
#define CANVAS_VIEW_H_

#include <stdint.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <cairo/cairo.h>

enum sotc_color {
    SOTC_COLOR_BG,
    SOTC_COLOR_GRID,
    SOTC_COLOR_NORMAL,
    SOTC_COLOR_ACCENT
};

enum sotc_paper_size {
    SOTC_PAPER_SIZE_INVALID,
    SOTC_PAPER_SIZE_A4,
    SOTC_PAPER_SIZE_A3,
    SOTC_PAPER_SIZE_A2,
    SOTC_PAPER_SIZE_A1,
};

enum sotc_state_resize_selector {
    SOTC_NO_SELECTION,
    SOTC_TOP_LEFT,
    SOTC_TOP_MIDDLE,
    SOTC_TOP_RIGHT,
    SOTC_RIGHT_MIDDLE,
    SOTC_BOT_RIGHT,
    SOTC_BOT_MIDDLE,
    SOTC_BOT_LEFT,
    SOTC_LEFT_MIDDLE
};

enum sotc_transition_vertice_selector {
    SOTC_TRANS_NO_SELECTION,
    SOTC_TRANS_BEGIN,
    SOTC_TRANS_VERT,
    SOTC_TRANS_END,
};

inline bool point_in_box(double px, double py,
                         double x, double y,
                         double w, double h)
{
    if ( (px > (x - w/2)) && (px < (x + w/2)) &&
         (py > (y - h/2)) && (py < (y + h/2))) {
        return true;
    }

    return false;
}

int sotc_color_set(cairo_t *cr, enum sotc_color color);

int sotc_canvas_render(cairo_t *cr, struct sotc_region *root,
                        int widht, int height);
int sotc_canvas_render_grid(cairo_t *cr, int width, int height);
int sotc_canvas_render_state(cairo_t *cr, struct sotc_state *state);
int sotc_canvas_render_region(cairo_t *cr, struct sotc_region *region);
int sotc_canvas_render_transition(cairo_t *cr, struct sotc_transition *t);
int sotc_canvas_scale(double zoom_change);
double sotc_canvas_get_scale(void);

int sotc_get_state_absolute_coords(struct sotc_state *s, double *x,
                                                         double *y,
                                                         double *w,
                                                         double *h);

int sotc_get_region_absolute_coords(struct sotc_region *r, double *x,
                                                           double *y,
                                                           double *w,
                                                           double *h);

bool sotc_region_is_root_or_offpage(struct sotc_region *r);

double sotc_canvas_nearest_grid_point(double in);
int sotc_canvas_state_translate(struct sotc_state *s, double dx, double dy);
int transition_calc_begin_end_point(struct sotc_state *s,
                                    enum sotc_side side,
                                    double offset,
                                    double *x,
                                    double *y);

int sotc_canvas_set_selection(bool active, double sx,
                                           double sy,
                                           double ex,
                                           double ey);
int sotc_canvas_pan(double dx, double dy);

int sotc_canvas_get_offset(double *x, double *y);

#endif  // CANVAS_VIEW_H_
