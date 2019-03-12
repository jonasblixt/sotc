#include <stdio.h>
#include <gui/menu.h>
#include <sb.h>
#include <nanovg.h>

struct menuitem
{
    char *name;
    bool enabled;
    uint32_t event;
}

struct menu
{
    double grid_x;
    double grid_y;
    double alpha;
};

uint32_t menu_create(struct component **c)
{
    if (c == NULL)
        return SB_ERR;

    (*c) = SB_MALLOC(sizeof(struct component));

    if (*c == NULL)
        return SB_ERR;

    (*c)->data = SB_MALLOC(sizeof(struct menu));

    return SB_OK;
}

uint32_t menu_add_item(struct component *c, const char name,
                       uint32_t event)
{

    return SB_OK;
}

uint32_t menu_destroy(struct component *c)
{
    if (c == NULL)
        return SB_ERR;

    if (c->data)
    {
        SB_FREE(c->data);
    }

    SB_FREE(c);

    return SB_OK;
}

uint32_t menu_render(NVGcontext* vg, struct component *c)
{
    struct menu *m = (struct menu *) c->data;


 
    return SB_OK;
}


