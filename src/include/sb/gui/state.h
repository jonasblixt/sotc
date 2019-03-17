#ifndef __SB_GUI_STATE_H__
#define __SB_GUI_STATE_H__

#include <stdint.h>
#include <nanovg.h>
#include <gui/component.h>

uint32_t state_create(struct component **c, const char *name);
uint32_t state_destroy(struct component *c);
uint32_t state_render(NVGcontext* vg, struct component *c);

#endif

