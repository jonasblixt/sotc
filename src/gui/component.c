#include <sb.h>
#include <stdio.h>
#include <nanovg.h>
#include <gui/component.h>
#include <gui/state.h>
#include <gui/grid.h>

bool component_render(NVGcontext* vg, struct component *c,int w, int h)
{
    switch (c->kind)
    {
        case SB_COMP_INVALID:
            return false;
        break;
        case SB_COMP_STATE:
            state_render(vg, c);
        break;
        case SB_COMP_GRID:
            grid_render(vg, c, w ,h);
        break;
        default:
            return false;
    }

    return true;
}
