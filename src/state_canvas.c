#include <sotc/sotc.h>

#include "state_canvas.h"

static void paint (GtkWidget *widget, GdkEventExpose *eev, gpointer data)
{
    gint width, height;
    gint i;
    cairo_t *cr;
    GtkAllocation allocation;

    printf("paint called!\n");

    gtk_widget_get_allocation(widget, &allocation);

    width = allocation.width;
    height = allocation.height;

    GdkWindow* window = gtk_widget_get_window(widget);
    cairo_region_t *cairoRegion = cairo_region_create();

    GdkDrawingContext *drawingContext = \
                 gdk_window_begin_draw_frame (window,cairoRegion);

    cr = gdk_drawing_context_get_cairo_context (drawingContext);

    /* clear background */
    cairo_set_source_rgb (cr, 1,1,1);
    cairo_paint (cr);


    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                        CAIRO_FONT_WEIGHT_BOLD);


    cairo_save(cr);
    /* Draw grid */
    for (gint x = 0; x < width/10; x++)
    {
        cairo_move_to (cr, x*10, 0);
        cairo_line_to (cr, x*10, height);
    }

    for (gint y = 0; y < height/10; y++)
    {
        cairo_move_to (cr, 0, y*10);
        cairo_line_to (cr, width, y*10);
    }

    cairo_set_source_rgb (cr, 0.9,0.9,0.9);
    cairo_set_line_width(cr, 1);
    cairo_stroke (cr);

    cairo_restore(cr);


    /* enclosing in a save/restore pair since we alter the
    * font size
    */
    cairo_save (cr);
    cairo_set_font_size (cr, 40);
    cairo_move_to (cr, 40, 60);
    cairo_set_source_rgb (cr, 0,0,0);
    cairo_show_text (cr, "State of the chart!");
    cairo_restore (cr);

    cairo_set_source_rgb (cr, 1,0,0);
    cairo_set_font_size (cr, 20);
    cairo_move_to (cr, 50, 100);
    cairo_show_text (cr, "Beep-boop boop");

    cairo_stroke (cr);

    gdk_window_end_draw_frame(window,drawingContext);
    cairo_region_destroy(cairoRegion);
}

int sotc_state_canvas_init(GtkWidget **canvas)
{
    GtkWidget *c = gtk_drawing_area_new();

    (*canvas) = c;

    g_signal_connect(G_OBJECT(c), "draw", G_CALLBACK(paint), NULL);

    return SOTC_OK;
}

int sotc_state_canvas_free(GtkWidget *canvas)
{
    return SOTC_OK;
}
