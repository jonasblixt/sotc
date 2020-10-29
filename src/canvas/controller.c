#include <math.h>
#include <time.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "canvas/controller.h"
#include "canvas/view.h"

static struct sotc_model *model;
static struct sotc_state *selected_state = NULL;
static struct sotc_region *selected_region = NULL;
static double selection_start_x, selection_start_y;
static double sselection_x, sselection_y;

static void draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    int rc;
    gint width, height;
    gint i;
    GtkAllocation allocation;
    struct timespec ts;
    uint64_t r_time;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    r_time = ts.tv_sec*1e3 + ts.tv_nsec / 1e6;

    gtk_widget_get_allocation(widget, &allocation);

    width = allocation.width;
    height = allocation.height;

    sotc_canvas_render(cr, model->root, width, height);

    clock_gettime(CLOCK_MONOTONIC, &ts);

    r_time = (ts.tv_sec*1e3 + ts.tv_nsec / 1e6) - r_time;
    printf("render %ld ms\n", r_time);
}

gboolean keypress_cb(GtkWidget *widget, GdkEventKey *event, gpointer data)
{

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

    if (event->keyval == GDK_KEY_x)
    {
        if (selected_state)
            selected_state->w += 10;
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_X)
    {
        if (selected_state)
            selected_state->w -= 10;
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_y)
    {
        if (selected_state)
            selected_state->h += 10;
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_Y)
    {
        if (selected_state)
            selected_state->h -= 10;
        gtk_widget_queue_draw (widget);
    }

    if (event->keyval == GDK_KEY_s)
    {
        printf("Saving...\n");
        sotc_model_write("out.sotc", model);
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
    double tx_tmp, ty_tmp;
    double sx, sy;
    static double tx, ty;

    sx = sotc_canvas_nearest_grid_point(selection_start_x);
    sy = sotc_canvas_nearest_grid_point(selection_start_y);

    if (selected_state && (event->state & GDK_BUTTON1_MASK)) {
        tx_tmp = sotc_canvas_nearest_grid_point(event->x);
        ty_tmp = sotc_canvas_nearest_grid_point(event->y);

        if (tx != tx_tmp || ty != ty_tmp) {
            if (fabs(tx - selection_start_x) > 10 ||
                fabs(ty - selection_start_y) > 10) {

                double dx = tx_tmp - sx;
                double dy = ty_tmp - sy;

                printf("move %s --> %f %f\n", selected_state->name, dx, dy);
                selected_state->x = sselection_x +
                                         (dx /** 1/sotc_canvas_get_scale()*/);
                selected_state->y = sselection_y +
                                         (dy /** 1/sotc_canvas_get_scale()*/);
                gtk_widget_queue_draw (widget);
            }
        }

        tx = tx_tmp;
        ty = ty_tmp;

    }

  return TRUE;
}
/*
static double distance_point_to_line(double px, double py,
                                     double sx, double sy,
                                     double ex, double ey)
{
    double a, b, c;

    a = sy - ey;
    b = ex - sx;
    c = sx * ey - ex * sy;

    printf("distance from <%f, %f> to line <<%f, %f>, <%f, %f>>\n",
                px, py, sx, sy, ex, ey);

    return fabs(a * px + b * py + c) / sqrt(a * a + b * b);
}
*/

static double distance_point_to_seg(double px, double py,
                                    double sx, double sy,
                                    double ex, double ey)
{
    double A = px - sx;
    double B = py - sy;
    double C = ex - sx;
    double D = ey - sy;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;

    if (len_sq != 0) //in case of 0 length line
        param = dot / len_sq;

    double xx, yy;

    if (param < 0) {
        xx = sx;
        yy = sy;
    } else if (param > 1) {
        xx = ex;
        yy = ey;
    } else {
        xx = sx + param * C;
        yy = sy + param * D;
    }

    double dx = px - xx;
    double dy = py - yy;

    printf("distance from <%f, %f> to line <<%f, %f>, <%f, %f>>\n",
            px, py, sx, sy, ex, ey);

    return sqrt(dx * dx + dy * dy);
}

gboolean buttonpress_cb(GtkWidget *widget, GdkEventButton *event)
{
    static struct sotc_stack *stack;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    double x, y, w, h;
    bool last_object_state = false;
    int rc;

    printf("%s %f %f\n", __func__,
                       event->x,
                       event->y);
    gtk_widget_grab_focus(widget);

    selected_state = NULL;
    selected_region = NULL;

    double px = event->x / sotc_canvas_get_scale();
    double py = event->y / sotc_canvas_get_scale();

    sotc_stack_init(&stack, SOTC_MAX_R_S);
    sotc_stack_push(stack, model->root);

    while (sotc_stack_pop(stack, (void **) &r) == SOTC_OK)
    {
        r->focus = false;
        sotc_get_region_absolute_coords(r, &x, &y, &w, &h);

        if ( (px > x) && (px < (x + w)) &&
             (py > y) && (py < (y + h))) {
             selected_region = r;
             last_object_state = false;
        }

        for (s = r->state; s; s = s->next)
        {
            s->focus = false;
            sotc_get_state_absolute_coords(s, &x, &y, &w, &h);

            if ( (px > x) && (px < (x + w)) &&
                 (py > y) && (py < (y + h))) {
                 selected_state = s;
                 last_object_state = true;
            }
            for (r2 = s->regions; r2; r2 = r2->next)
            {
                if (r->off_page)
                    continue;

                sotc_stack_push(stack, r2);
            }
        }
    }


    if (selected_state && last_object_state) {
         L_DEBUG("State %s selected, pr = %s", selected_state->name,
                    selected_state->parent_region->name);
         selected_state->focus = true;
         selection_start_y = event->y;
         selection_start_x = event->x;
         sselection_x = selected_state->x;
         sselection_y = selected_state->y;
         selected_region = NULL;
    } else if (selected_region && !last_object_state) {
         L_DEBUG("Region %s selected!", selected_region->name);
         selected_region->focus = true;
         selected_state = NULL;
    }

    /* Check transitions selection */

    sotc_stack_push(stack, model->root);

    while (sotc_stack_pop(stack, (void **) &r) == SOTC_OK)
    {
        for (s = r->state; s; s = s->next)
        {
            if (s->transition) {
                struct sotc_vertice *v;
                double vsx, vsy, vex, vey;
                double tsx, tsy, tex, tey;
                double d;
                bool t_focus = false;
                L_DEBUG("Checking transitions from %s", s->name);
                s->transition->focus = false;
                transition_calc_begin_end_point(s,
                                                s->transition->source.side,
                                                s->transition->source.offset,
                                                &tsx, &tsy);
                transition_calc_begin_end_point(s->transition->dest.state,
                                                s->transition->dest.side,
                                                s->transition->dest.offset,
                                                &tex, &tey);
                vsx = tsx;
                vsy = tsy;

                for (v = s->transition->vertices; v; v = v->next) {
                    vex = v->x;
                    vey = v->y;

                    d = distance_point_to_seg(px, py,
                                              vsx, vsy,
                                              vex, vey);
                    printf("d = %f\n", d);
                    if (d < 10.0) {
                        t_focus = true;
                        break;
                    }
                    vsx = v->x;
                    vsy = v->y;
                }
                vsx = vex;
                vsy = vey;
                vex = tex;
                vey = tey;

                d = distance_point_to_seg(px, py,
                                          vsx, vsy,
                                          vex, vey);
                printf("d = %f\n", d);
                if (d < 10.0)
                    t_focus = true;

                if (t_focus) {
                    printf("Transition is focused!\n");
                    s->transition->focus = true;
                    if (selected_state)
                        selected_state->focus = false;
                    if (selected_region)
                        selected_region->focus = false;
                }
            }
            for (r2 = s->regions; r2; r2 = r2->next)
            {
                if (r->off_page)
                    continue;

                sotc_stack_push(stack, r2);
            }
        }
    }

    sotc_stack_free(stack);
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
