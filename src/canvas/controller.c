#include <math.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/controller.h"
#include "canvas/view.h"

static struct sotc_model *model;

static void draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    int rc;
    gint width, height;
    gint i;
    GtkAllocation allocation;

    printf("Draw!\n");

    gtk_widget_get_allocation(widget, &allocation);

    width = allocation.width;
    height = allocation.height;

    sotc_canvas_render(cr, model->root, width, height);
}

gboolean keypress_cb(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    printf("%s\n", __func__);

    if (event->keyval == GDK_KEY_a)
    {
        sotc_canvas_scale(0.1);
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_b)
    {
        sotc_canvas_scale(-0.1);
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_space)
    {
        printf("SPACE KEY PRESSED!\n");
        return TRUE;
    }

    if (event->keyval == GDK_KEY_Shift_L)
    {
        printf("Shift KEY PRESSED!\n");
        return TRUE;
    }

    return TRUE;
}


static gboolean motion_notify_event_cb (GtkWidget      *widget,
                                        GdkEventMotion *event,
                                        gpointer        data)
{

    //printf("%s\n", __func__);
  /*if (event->state & GDK_BUTTON1_MASK)
    state_canvas_draw (widget, event->x, event->y);
*/
  /* We've handled it, stop processing */
  return TRUE;
}

gboolean buttonpress_cb(GtkWidget *widget, GdkEventButton *event)
{
    static struct sotc_stack *stack;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    struct sotc_state *selected_state = NULL;
    struct sotc_region *selected_region = NULL;
    bool last_object_state = false;
    int rc;

    printf("%s %f %f\n", __func__,
                       event->x,
                       event->y);
    gtk_widget_grab_focus(widget);

    int x = (int) event->x;
    int y = (int) event->y;

    sotc_stack_init(&stack, SOTC_MAX_R_S);
    sotc_stack_push(stack, model->root);

    while (sotc_stack_pop(stack, (void **) &r) == SOTC_OK)
    {
        r->focus = false;

        if ( (x > r->x) && (x < (r->x + r->w)) &&
             (y > r->y) && (y < (r->y + r->h))) {
             selected_region = r;
             last_object_state = false;
        }

        for (s = r->state; s; s = s->next)
        {
            s->focus = false;

            if ( (x > s->x) && (x < (s->x + s->w)) &&
                 (y > s->y) && (y < (s->y + s->h))) {
                 selected_state = s;
                 last_object_state = true;
            }
            for (r2 = s->regions; r2; r2 = r2->next)
            {
                sotc_stack_push(stack, r2);
            }
        }
    }

    sotc_stack_free(stack);

    if (selected_state && last_object_state) {
         L_DEBUG("State %s selected!", selected_state->name);
         selected_state->focus = true;
    } else if (selected_region && !last_object_state) {
         L_DEBUG("Region %s selected!", selected_region->name);
         selected_region->focus = true;
    }

    gtk_widget_queue_draw (widget);

    return TRUE;
}

int sotc_state_canvas_init(GtkWidget **canvas)
{
    GtkWidget *c = gtk_drawing_area_new();
    (*canvas) = c;

    gtk_widget_set_events (c, gtk_widget_get_events (c)
                                     | GDK_BUTTON_PRESS_MASK
                                     | GDK_POINTER_MOTION_MASK);


    /* Event signals */
    g_signal_connect(G_OBJECT(c), "key_press_event",
                     G_CALLBACK (keypress_cb), NULL);

    g_signal_connect(G_OBJECT(c), "button_press_event",
                     G_CALLBACK (buttonpress_cb), NULL);


    g_signal_connect (G_OBJECT(c), "motion-notify-event",
                    G_CALLBACK (motion_notify_event_cb), NULL);

    g_signal_connect(G_OBJECT(c), "draw", G_CALLBACK(draw_cb), NULL);

    gtk_widget_set_can_focus(c, TRUE);
    gtk_widget_set_focus_on_click(c, TRUE);
    gtk_widget_grab_focus(c);

    return SOTC_OK;
}

int sotc_state_canvas_free(GtkWidget *canvas)
{
    return SOTC_OK;
}


int sotc_state_canvas_update(struct sotc_model *model_,
                             struct sotc_region *region)
{
    model = model_;
    return SOTC_OK;
}
