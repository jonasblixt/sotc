#include <stdio.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#ifdef NANOVG_GLEW
#    include <GL/glew.h>
#endif
#ifdef __APPLE__
#    define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include "render.h"

static GLFWwindow* window;

void errorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}


static void mouse_button_callback(GLFWwindow* window, int button,
                                        int action, int mods)
{
}

static void cursor_position_callback(GLFWwindow* window,
                                     double xpos,
                                     double ypos)
{
}


static void window_size_callback(GLFWwindow* window, int width, int height)
{
    printf("Window resize event: %u %u\n",width, height);
}

int main(int argc, char **argv)
{
    int rc;
    char banner[128];
    NVGcontext* vg = NULL;


    if (!glfwInit())
    {
        printf("Failed to init GLFW.");
        return -1;
    }

    snprintf(banner, sizeof(banner), "The Credible Machine v%s", PACKAGE_VERSION);

    glfwSetErrorCallback(errorcb);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    window = glfwCreateWindow(1000, 600, banner, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwMakeContextCurrent(window);

    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

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
    glfwSwapInterval(0);

    /* Main render loop */
    while (!glfwWindowShouldClose(window))
    {
        double mx, my, t;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        float pxRatio;

        t = glfwGetTime();

        glfwGetCursorPos(window, &mx, &my);
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float)fbWidth / (float)winWidth;

        // Update and render
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(1.0f, 1.0f, 1.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

        tcm_render(model->root, vg);

        nvgEndFrame(vg);
        glfwSwapBuffers(window);
        glfwWaitEventsTimeout(0.5);

    }

    printf("Clean-up...\n");
    tcm_render_free();
    tcm_model_free(model);

err_out:
    nvgDeleteGL3(vg);
    glfwTerminate();
    return rc;
}
