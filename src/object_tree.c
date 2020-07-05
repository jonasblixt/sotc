#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "object_tree.h"

static GtkTreeStore *store;
static struct sotc_stack *stack;

int sotc_object_tree_init(GtkWidget **widget)
{
    GtkWidget *tree;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    int rc;

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    /* Create a model.  We are using the store model for now, though we
    * could use any other GtkTreeModel */
    store = gtk_tree_store_new (1, G_TYPE_STRING);

    /* custom function to fill the model with data */
    //populate_tree_model (store);
    //
/*
    GtkTreeIter iter1, iter2;

    gtk_tree_store_append (store, &iter1, NULL);
    gtk_tree_store_set (store, &iter1,
                    0, "Root region",
                    -1);

    gtk_tree_store_append (store, &iter2, &iter1);
    gtk_tree_store_set (store, &iter2,
                        0, "State",
                        -1);

    gtk_tree_store_append (store, &iter2, &iter1);
    gtk_tree_store_set (store, &iter2,
                        0, "State2",
                        -1);
*/
    /* Create a view */
    tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

    gtk_tree_view_set_show_expanders(tree, TRUE);
    gtk_tree_view_set_headers_visible(tree, FALSE);
    gtk_tree_view_set_expander_column(tree, NULL);

    /* The view now holds a reference.  We can get rid of our own
    * reference */
    g_object_unref (G_OBJECT (store));

    /* Create a cell render and arbitrarily make it red for demonstration
    * purposes */
    renderer = gtk_cell_renderer_text_new ();

    /* Create a column, associating the "text" attribute of the
    * cell_renderer to the first column of the model */
    column = gtk_tree_view_column_new_with_attributes ("Object tree", renderer,
                                                      "text", 0,
                                                      NULL);

    /* Add the column to the view. */
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

    (*widget) = tree;

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
    int rc;
    GtkTreeIter *s_iter = NULL;
    GtkTreeIter r_iter;
    GtkTreeIter *parent = NULL;

    printf("Updating tree <%p>\n", model->root);
    rc = stack_push_r_iter_pair(stack, model->root, NULL);
    printf("rc = %i\n", rc);

    printf("Iterating...\n");
    while (stack_pop_r_iter_pair(stack, &r, &parent) == SOTC_OK)
    {
        printf("pop'ed <%p>\n", r);
        printf("R %s <%i, %i, %i, %i>\n", r->name, r->x, r->y, r->w, r->h);

        gtk_tree_store_append (store, &r_iter, parent);
        gtk_tree_store_set (store, &r_iter, 0, r->name, -1);

        for (s = r->state; s; s = s->next)
        {
            printf("S %s\n", s->name);

            s_iter = malloc(sizeof(*s_iter));

            gtk_tree_store_append (store, s_iter, &r_iter);
            gtk_tree_store_set (store, s_iter, 0, s->name, -1);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                stack_push_r_iter_pair(stack, r2, s_iter);
            }
        }
    }

    return SOTC_OK;
}

