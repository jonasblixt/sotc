#include <sotc/sotc.h>

#include "object_tree.h"

int sotc_object_tree_init(GtkWidget **widget)
{
    GtkTreeStore *store;
    GtkWidget *tree;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    /* Create a model.  We are using the store model for now, though we
    * could use any other GtkTreeModel */
    store = gtk_tree_store_new (1, G_TYPE_STRING);

    /* custom function to fill the model with data */
    //populate_tree_model (store);
    //
    GtkTreeIter iter1, iter2;

    gtk_tree_store_append (store, &iter1, NULL);
    gtk_tree_store_set (store, &iter1,
                    0, "Root region",
                    -1);

    gtk_tree_store_append (store, &iter2, &iter1);  /* Acquire a child iterator */
    gtk_tree_store_set (store, &iter2,
                        0, "State",
                        -1);

    gtk_tree_store_append (store, &iter2, &iter1);  /* Acquire a child iterator */
    gtk_tree_store_set (store, &iter2,
                        0, "State2",
                        -1);
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

