#ifndef __SB_GUI_STATE_H__
#define __SB_GUI_STATE_H__

#include <stdint.h>
#include <nanovg.h>

struct sb_state
{
    char *name;
    float x,y,z,w,h;
};

uint32_t render_state(NVGcontext* vg, struct sb_state *s);

#endif

