#include <stdio.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <SDL2/SDL.h>
#include <nanovg.h>

#ifdef OSX
#include <nanovg_mtl.h>
#endif

#include "render.h"

int main(int argc, char **argv)
{
    int rc;
    char banner[128];
    NVGcontext* vg = NULL;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Getting Started", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event input;

#ifdef OSX
    vg = nvgCreateMTL(SDL_RenderGetMetalLayer(renderer),
                        NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
 //   vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    SDL_RendererInfo *rend_info = (SDL_RendererInfo *) malloc(sizeof(SDL_RendererInfo));
            SDL_GetRendererInfo(renderer, rend_info);

    printf("SDL Renderer: %s\n", rend_info->name);


    if (vg == NULL)
    {
        printf("Could not init nanovg.\n");
        return -1;
    }

    int font_bold =
        nvgCreateFont(vg, "bold", "fonts/Hack Bold Nerd Font Complete.ttf");

    if (font_bold == -1)
    {
        printf("Error: Could not load font\n");
        return -1;
    }

    int font_italic =
        nvgCreateFont(vg, "italic", "fonts/Hack Italic Nerd Font Complete.ttf");

    if (font_italic == -1)
    {
        printf("Error: Could not load font\n");
        return -1;
    }

    int font_regular =
        nvgCreateFont(vg, "regular", "fonts/Hack Regular Nerd Font Complete.ttf");

    if (font_regular == -1)
    {
        printf("Error: Could not load font\n");
        return -1;
    }
    struct tcm_model *model;
    rc = tcm_model_load(argv[1], &model);

    if (rc != TCM_OK)
    {
        printf("Could not load model\n");
        goto err_out;
    }

    tcm_render_init();

    /* Main render loop */
    bool quit = false;

    while (!quit)
    {
 /*       SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
*/

        nvgBeginFrame(vg, 800, 600, 1.0);
        tcm_render(model->root, vg);

	nvgSave(vg);
	nvgBeginPath(vg);
	nvgRoundedRect(vg, 10, 10, 300, 300, 0);
	nvgStrokeColor(vg, nvgRGBA(50, 50, 50, 255));
    nvgStrokeWidth(vg, 0.5);
	nvgStroke(vg);
	nvgRestore(vg);
        nvgEndFrame(vg);

/*
        SDL_RenderDrawLine(renderer, 10, 10, 200, 200);
        SDL_RenderPresent(renderer);
*/
        while (SDL_PollEvent(&input) > 0)
        {
            printf("Input ev: %i\n", input.type);

            if (input.type == SDL_QUIT) 
                quit = true;

        }
    }

    printf("Clean-up...\n");
    tcm_render_free();
    tcm_model_free(model);

err_out:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return rc;
}
