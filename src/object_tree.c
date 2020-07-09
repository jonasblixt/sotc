#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "object_tree.h"

enum
{
    COL_TEXT = 0,
    NUM_COLS
};

static GtkTreeStore *store;

int sotc_object_tree_init(GtkWidget **widget)
{
    GtkTreeView *tree;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    int rc;

    store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
    tree = (GtkTreeView *) gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    gtk_tree_view_set_show_expanders(tree, TRUE);
    gtk_tree_view_set_headers_visible(tree, FALSE);
    gtk_tree_view_set_enable_tree_lines(tree, TRUE);

    g_object_unref(G_OBJECT(store));

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Object tree", renderer,
                                                      "text", COL_TEXT,
                                                      NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

    (*widget) = (GtkWidget *) tree;

    return SOTC_OK;
}

int sotc_object_tree_free(GtkWidget *widget)
{
    return SOTC_OK;
}

static int stack_push_r_iter_pair(struct sotc_stack *stack,
                                  struct sotc_region *r,
                                  GtkTreeIter *iter)
{
    int rc;

    rc = sotc_stack_push(stack, r);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_push(stack, iter);
}

static int stack_pop_r_iter_pair(struct sotc_stack *stack,
                                 struct sotc_region **r,
                                 GtkTreeIter **iter)
{
    int rc;

    rc = sotc_stack_pop(stack, (void **) iter);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_pop(stack, (void **) r);
}

int sotc_object_tree_update(struct sotc_model *model)
{
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    static struct sotc_stack *stack;
    struct sotc_stack *cleanup_stack;
    int rc;
    GtkTreeIter *s_iter = NULL;
    GtkTreeIter r_iter;
    GtkTreeIter *parent = NULL;

    gchar *icon_test = "battery-caution-charging-symbolic";
    rc = sotc_stack_init(&cleanup_stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    rc = stack_push_r_iter_pair(stack, model->root, NULL);

    while (stack_pop_r_iter_pair(stack, &r, &parent) == SOTC_OK)
    {
        printf("R %s <%i, %i, %i, %i>\n", r->name, r->x, r->y, r->w, r->h);

        gtk_tree_store_append(store, &r_iter, parent);
        gtk_tree_store_set(store, &r_iter, COL_TEXT, r->name,
                                           -1);

        for (s = r->state; s; s = s->next)
        {
            printf("S %s\n", s->name);

            s_iter = malloc(sizeof(*s_iter));
            sotc_stack_push(cleanup_stack, (void *) s_iter);

            gtk_tree_store_append(store, s_iter, &r_iter);
            gtk_tree_store_set(store, s_iter, COL_TEXT, s->name,
                                              -1);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                stack_push_r_iter_pair(stack, r2, s_iter);
            }
        }
    }

    /* Free temprary iterators */
    void *p;

    while(sotc_stack_pop(cleanup_stack, &p) == SOTC_OK)
    {
        L_DEBUG("Freeing temporary iterator <%p>", p);
        free(p);
    }

    sotc_stack_free(cleanup_stack);
    sotc_stack_free(stack);

    return SOTC_OK;
}

