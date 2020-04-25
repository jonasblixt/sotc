#ifndef __SB_GUI_COMPONENT_H__
#define __SB_GUI_COMPONENT_H__

#include <stdint.h>
#include <nanovg.h>
#include <stdbool.h>

enum
{
    SB_COMP_INVALID,
    SB_COMP_STATE,
    SB_COMP_GRID,
    SB_COMP_TEXTBOX,
    SB_COMP_MENU
};

struct component
{
    unsigned int kind;
    double x;
    double y;
    double z;
    double w;
    double h;
    double alpha;
    bool selected;
    struct component *parent;
    struct component *child;
    struct component *next;
    struct component *prev;
    void * data;
};

bool component_render(NVGcontext* vg, struct component *c,int w, int h);
bool component_hotzone_selected(struct component *c, double mx, double my);

#endif
