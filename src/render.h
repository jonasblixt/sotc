#ifndef RENDER_H_
#define RENDER_H_

#include <sotc/sotc.h>
#include <sotc/model.h>

int sotc_render(struct sotc_region *region);

int sotc_render_init(void);

int sotc_render_free(void);

#endif  // RENDER_H_
