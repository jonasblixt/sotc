#include <stdio.h>
#include <gui/state.h>
#include <nanovg.h>
#include <stdint.h>
#include <stdbool.h>


uint32_t render_state(NVGcontext* vg, struct sb_state *s)
{
	float cornerRadius = 10.0f;
	NVGpaint shadowPaint;
	NVGpaint headerPaint;

	nvgSave(vg);

    /* Draw box */
	nvgBeginPath(vg);
	nvgRoundedRect(vg, s->x,s->y,s->w,s->h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(255,255,255,255));
	nvgFill(vg);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStrokeWidth(vg, 2.0);
	nvgStroke(vg);

    /* Draw top label */
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "regular");
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

	//nvgFontBlur(vg,0);
	nvgFillColor(vg, nvgRGBA(0,0,0,255));
	nvgText(vg, s->x+s->w/2,s->y+16, s->name, NULL);

    /* Draw state top line */
	nvgBeginPath(vg);
	nvgMoveTo(vg, s->x,s->y+30);
	nvgLineTo(vg, s->x+s->w,s->y+30);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
    nvgStrokeWidth(vg, 2.0);
	nvgStroke(vg);
    /* Draw state top line end */

	nvgRestore(vg);
}
