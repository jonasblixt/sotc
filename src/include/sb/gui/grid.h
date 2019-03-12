#ifndef __SB_GUI_GRID_H__
#define __SB_GUI_GRID_H__

#include <gui/component.h>

uint32_t grid_create(struct component **c);
uint32_t grid_destroy(struct component *c);
uint32_t grid_render(NVGcontext* vg,struct component *c);
uint32_t grid_set_spacing(struct component *c, double spacing);

#endif
