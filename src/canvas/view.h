#ifndef CANVAS_VIEW_H_
#define CANVAS_VIEW_H_

#include <stdint.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <cairo/cairo.h>

int sotc_canvas_render(cairo_t *cr, struct sotc_region *root,
                        int widht, int height);
int sotc_canvas_render_grid(cairo_t *cr, int width, int height);
int sotc_canvas_render_state(cairo_t *cr, struct sotc_state *state);
int sotc_canvas_render_region(cairo_t *cr, struct sotc_region *region);
int sotc_canvas_scale(double zoom_change);

int sotc_get_state_absolute_coords(struct sotc_state *s, double *x,
                                                         double *y,
                                                         double *w,
                                                         double *h);

int sotc_get_region_absolute_coords(struct sotc_region *r, double *x,
                                                           double *y,
                                                           double *w,
                                                           double *h);

bool sotc_region_is_root_or_offpage(struct sotc_region *r);

#endif  // CANVAS_VIEW_H_
