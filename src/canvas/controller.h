#ifndef CANVAS_CONTROLLER_H_
#define CANVAS_CONTROLLER_H_

#include <gtk/gtk.h>
#include <sotc/sotc.h>
#include <sotc/model.h>

int sotc_state_canvas_init(GtkWidget **canvas);
int sotc_state_canvas_free(GtkWidget *canvas);
int sotc_state_canvas_update(struct sotc_model *model,
                             struct sotc_region *region);

#endif  // CANVAS_CONTROLLER_H_
