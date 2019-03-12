#ifndef __SB_GUI_COMPONENT_H__
#define __SB_GUI_COMPONENT_H__

#include <stdint.h>
#include <nanovg.h>

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
    struct component *parent;
    struct component *child;
    struct component *next;
    struct component *prev;
    void * data;
};

#endif
