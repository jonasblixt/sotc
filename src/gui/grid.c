#include <stdio.h>
#include <gui/grid.h>
#include <sb.h>
#include <nanovg.h>

struct grid
{
    double grid_x;
    double grid_y;
    double alpha;
};

uint32_t grid_create(struct component **c)
{
    if (c == NULL)
        return SB_ERR;

    (*c) = SB_MALLOC(sizeof(struct component));

    if (*c == NULL)
        return SB_ERR;

    (*c)->data = SB_MALLOC(sizeof(struct grid));

    struct grid *g = (struct grid *) (*c)->data;
    g->grid_x = 30;
    g->grid_y = 30;
    g->alpha = 255;
    (*c)->kind = SB_COMP_GRID;
    (*c)->next = NULL;

    return SB_OK;
}

uint32_t grid_destroy(struct component *c)
{
    if (c == NULL)
        return SB_ERR;

    if (c->data)
        SB_FREE(c->data);
    SB_FREE(c);

    return SB_OK;
}

uint32_t grid_render(NVGcontext* vg, struct component *c, int width,
                                                          int height)
{
    struct grid *g = (struct grid *) c->data;

    int x_steps = (width/g->grid_x)+1;
    int y_steps = (height/g->grid_y)+1;

	nvgSave(vg);

    for (uint32_t i = 0; i < y_steps; i++)
    {
        nvgBeginPath(vg);
        nvgMoveTo(vg, 0,i*g->grid_y);
        nvgLineTo(vg, width,i*g->grid_y);
        nvgStrokeColor(vg, nvgRGBA(230,230,230,g->alpha));
        nvgStrokeWidth(vg, 1.0);
        nvgStroke(vg);
    }

    for (uint32_t i = 0; i < x_steps; i++)
    {
        nvgBeginPath(vg);
        nvgMoveTo(vg, i*g->grid_x,0);
        nvgLineTo(vg, i*g->grid_x,height);
        nvgStrokeColor(vg, nvgRGBA(230,230,230,g->alpha));
        nvgStrokeWidth(vg, 1.0);
        nvgStroke(vg);
    }
 
    return SB_OK;
}

uint32_t grid_set_spacing(struct component *c, double spacing)
{
    struct grid *g = (struct grid *) c->data;
    g->grid_x = spacing;
    g->grid_y = spacing;
    return SB_OK;
}
