#include <stdio.h>
#include <string.h>
#include <gui/menu.h>
#include <sb.h>
#include <nanovg.h>

struct menuitem
{
    char *name;
    bool enabled;
    uint32_t event;
    struct menuitem *next;
};

struct menu
{
    struct menuitem *menu;
    struct menuitem *last;
};

uint32_t menu_create(struct component **c)
{
    if (c == NULL)
        return SB_ERR;

    (*c) = SB_MALLOC(sizeof(struct component));
    bzero(*c, sizeof(struct component));

    if (*c == NULL)
        return SB_ERR;

    (*c)->data = SB_MALLOC(sizeof(struct menu));
    bzero((*c)->data, sizeof(struct menu));

    return SB_OK;
}

uint32_t menu_add_item(struct component *c, const char *name,
                       uint32_t event)
{
    if (c == NULL)
        return SB_ERR;

    struct menu *menu = (struct menu *) c->data;
    struct menuitem *m = SB_MALLOC(sizeof(struct menuitem));
    bzero(m,sizeof(struct menuitem));
    m->name = SB_MALLOC(strlen(name)+1);
    strcpy(m->name, name);
    m->event = event;
    m->enabled = true;

    if (menu->menu == NULL)
    {
        menu->menu = m;
    }
    else
    {
        menu->last->next = m;
    }

    menu->last = m;

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
 
	nvgSave(vg);

    /* Draw box */
	nvgBeginPath(vg);
	nvgRoundedRect(vg, c->x,c->y,c->w,c->h, 3);
	nvgFillColor(vg, nvgRGBA(230,230,230,200));
	nvgFill(vg);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStrokeWidth(vg, 1.0);
	nvgStroke(vg);

    /* Draw top label */
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "regular");
	nvgTextAlign(vg,NVG_ALIGN_LEFT);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));

    int n = 1;

	nvgText(vg, c->x+15,c->y+n*25, "ADD", NULL);
    n = n + 1;

    for (struct menuitem *mi = m->menu; mi; mi = mi->next)
    {
        n = n + 1;
	    nvgText(vg, c->x+15,c->y+25*n, mi->name, NULL);
    }
	nvgRestore(vg);
    return SB_OK;
}


