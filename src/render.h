#ifndef RENDER_H_
#define RENDER_H_

#include <tcm/tcm.h>
#include <tcm/model.h>
#include <nanovg.h>

int tcm_render(struct tcm_region *region,
               NVGcontext* vg);

int tcm_render_init(void);

int tcm_render_free(void);

#endif  // RENDER_H_
