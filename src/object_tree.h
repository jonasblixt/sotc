#ifndef OBJECT_TREE_H_
#define OBJECT_TREE_H_

#include <sotc/sotc.h>
#include <gtk/gtk.h>

int sotc_object_tree_init(GtkWidget **widget);
int sotc_object_tree_free(GtkWidget *widget);
int sotc_object_tree_update(struct sotc_model *model);

#endif  // OBJECT_TREE_H_
