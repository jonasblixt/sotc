#include <stdio.h>
#include <sotc/tcm.h>
#include <sotc/model.h>
#include <sotc/stack.h>

#include "render.h"

static struct sotc_stack *stack;

static int render_region(NVGcontext *vg, struct sotc_region *region)
{
	nvgSave(vg);
	nvgBeginPath(vg);
	nvgRoundedRect(vg, region->x, region->y, region->w, region->h, 0);
	nvgStrokeColor(vg, nvgRGBA(50, 50, 50, 255));
    nvgStrokeWidth(vg, 0.5);
	nvgStroke(vg);
	nvgRestore(vg);
    return 0;
}

static int render_state(NVGcontext* vg, struct sotc_state *state)
{
	float cornerRadius = 10.0f;

	nvgSave(vg);

    /* Draw box */
	nvgBeginPath(vg);
	nvgRoundedRect(vg, state->x, state->y, state->w, state->h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(255,255,255,255));
	nvgFill(vg);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStrokeWidth(vg, 1.0);
	nvgStroke(vg);

    /* Draw top label */
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "regular");
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgText(vg, state->x + state->w/2, state->y + 16, state->name, NULL);

    /* Draw state top line */
	nvgBeginPath(vg);
	nvgMoveTo(vg, state->x, state->y+30);
	nvgLineTo(vg, state->x + state->w, state->y+30);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStrokeWidth(vg, 1.0);
	nvgStroke(vg);
    /* Draw state top line end */
#ifdef __NOPE
    if (c->selected)
    {
        int x,y,w,h;
        x = c->x-1;
        y = c->y-1;
        w = c->w+2;
        h = c->h+2;

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x,y,w,h, 0);
        /* Top hot zones */
        nvgRoundedRect(vg, x-10,y-10,10,10, 0);
        nvgRoundedRect(vg, x+w,y-10,10,10, 0);
        /* Bottom */
        nvgRoundedRect(vg, x-10,y+h,10,10, 0);
        nvgRoundedRect(vg, x+w,y+h,10,10, 0);
        /* Middle */

        nvgRoundedRect(vg, x-10,y+h/2-5,10,10, 0);
        nvgRoundedRect(vg, x+w,y+h/2-5,10,10, 0);

        nvgStrokeColor(vg, nvgRGBA(150,150,200,255*c->alpha));
        nvgStrokeWidth(vg, 1.0);
        nvgStroke(vg);
    }
#endif
	nvgRestore(vg);
    return sotc_OK;
}

int sotc_render_init(void)
{
    int rc;

    rc = sotc_stack_init(&stack, sotc_MAX_R_S);

    if (rc != sotc_OK)
        return rc;

    return sotc_OK;
}

int sotc_render_free(void)
{
    sotc_stack_free(stack);
    return sotc_OK;
}

int sotc_render(struct sotc_region *region, NVGcontext* vg)
{
    struct sotc_region *r, *r2;
    struct sotc_state *s;

    sotc_stack_push(stack, region);

    while (sotc_stack_pop(stack, (void **) &r) == sotc_OK)
    {
        printf("R %s <%i, %i, %i, %i>\n", r->name, r->x, r->y, r->w, r->h);
        render_region(vg, r);

        for (s = r->state; s; s = s->next)
        {
            printf("S %s\n", s->name);
            render_state(vg, s);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

    return sotc_OK;
}
