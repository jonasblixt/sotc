#include <stdio.h>
#include <sotc/sotc.h>
#include <sotc/model.h>

#include <gtk/gtk.h>
#include <cairo.h>

#include "state_canvas.h"

int main(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *state_canvas;
    GtkWidget *object_tree;

    int rc = SOTC_OK;
    struct sotc_model *model;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "State of the chart - " PACKAGE_VERSION);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    rc = sotc_state_canvas_init(&state_canvas);

    if (rc != SOTC_OK)
    {
        L_ERR("Could not initialize state drawing canvas");
        goto err_out;
    }


    GtkWidget *vpane_toolbox = gtk_paned_new(GTK_ORIENTATION_VERTICAL);

    //GtkWidget *object_tree = gtk_label_new("Object tree");
    
    rc = sotc_object_tree_init(&object_tree);

    if (rc != SOTC_OK)
    {
        L_ERR("Could not initialize object tree");
        goto err_out;
    }

    gtk_widget_set_size_request(object_tree, 200, 500);

    GtkWidget *toolbox = gtk_label_new("Toolbox");
    gtk_widget_set_size_request(toolbox, -1, 200);

    gtk_paned_add1(GTK_PANED(vpane_toolbox), object_tree);
    gtk_paned_add2(GTK_PANED(vpane_toolbox), toolbox);

    GtkWidget *props = gtk_label_new("Object properties");

    GtkWidget *hpane_props = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_size_request(state_canvas, 200, 200);
    gtk_paned_add1(GTK_PANED(hpane_props), state_canvas);
    gtk_paned_add2(GTK_PANED(hpane_props), props);

    GtkWidget *hpane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);

    gtk_paned_add1(GTK_PANED(hpane), vpane_toolbox);
    gtk_paned_add2(GTK_PANED(hpane), hpane_props);

    gtk_container_add(GTK_CONTAINER(window), hpane);

    rc = sotc_model_load("test/test_model_out.sotc", &model);

    if (rc != SOTC_OK)
    {
        printf("Could not load model\n");
        goto err_out;
    }

    sotc_object_tree_update(model);


    gtk_widget_show_all(window);
    gtk_main();


    printf("Clean-up...\n");
    sotc_model_free(model);


err_out:
    return rc;
}
