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

uint32_t grid_render(NVGcontext* vg, struct component *c)
{
    struct grid *g = (struct grid *) c->data;

	nvgSave(vg);

    for (uint32_t i = 0; i < 20; i++)
    {
        nvgBeginPath(vg);
        nvgMoveTo(vg, 0,i*30);
        nvgLineTo(vg, 500,i*30);
        nvgStrokeColor(vg, nvgRGBA(230,230,230,255));
        nvgStrokeWidth(vg, 1.0);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgMoveTo(vg, i*30,0);
        nvgLineTo(vg, i*30,500);
        nvgStrokeColor(vg, nvgRGBA(230,230,230,255));
        nvgStrokeWidth(vg, 1.0);
        nvgStroke(vg);
    }
 
    return SB_OK;
}

uint32_t grid_set_spacing(struct component *c, double spacing)
{
    struct grid *g = (struct grid *) c->data;

    return SB_OK;
}
