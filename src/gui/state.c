#include <stdio.h>
#include <gui/state.h>
#include <nanovg.h>
#include <stdint.h>
#include <sb.h>
#include <stdbool.h>

struct state
{
    char *name;
};

uint32_t state_create(struct component **c, const char *name)
{
    if (c == NULL)
        return SB_ERR;

    (*c) = SB_MALLOC(sizeof(struct component));
    bzero(*c,sizeof(struct component));

    if (*c == NULL)
        return SB_ERR;

    (*c)->data = SB_MALLOC(sizeof(struct state));

    struct state *s = (struct state *) (*c)->data;
    bzero(s,sizeof(struct state));
    s->name = SB_MALLOC(strlen(name) + 1);
    strcpy(s->name, name);

    (*c)->alpha = 1.0;
    (*c)->kind = SB_COMP_STATE;
    (*c)->next = NULL;


    return SB_OK;
}

uint32_t state_destroy(struct component *c)
{

}

uint32_t state_render(NVGcontext* vg, struct component *c)
{
    struct state *s = (struct state *) c->data;

	float cornerRadius = 10.0f;

	nvgSave(vg);

    /* Draw box */
	nvgBeginPath(vg);
	nvgRoundedRect(vg, c->x,c->y,c->w,c->h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(255,255,255,255*c->alpha));
	nvgFill(vg);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255*c->alpha));
    nvgStrokeWidth(vg, 1.0);
	nvgStroke(vg);

    /* Draw top label */
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "regular");
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

	nvgFillColor(vg, nvgRGBA(0,0,0,255*c->alpha));
	nvgText(vg, c->x+c->w/2,c->y+16, s->name, NULL);

    /* Draw state top line */
	nvgBeginPath(vg);
	nvgMoveTo(vg, c->x,c->y+30);
	nvgLineTo(vg, c->x+c->w,c->y+30);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255*c->alpha));
    nvgStrokeWidth(vg, 1.0);
	nvgStroke(vg);
    /* Draw state top line end */

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
	nvgRestore(vg);
}
