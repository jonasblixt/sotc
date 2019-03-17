#ifndef __SB_GUI_MENU_H__
#define __SB_GUI_MENU_H__

#include <gui/component.h>

uint32_t menu_create(struct component **c);
uint32_t menu_destroy(struct component *c);
uint32_t menu_render(NVGcontext* vg,struct component *c);
uint32_t menu_add_item(struct component *c, const char *name,
                       uint32_t event);

#endif
