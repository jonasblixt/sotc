#include <stdio.h>
#ifdef NANOVG_GLEW
#    include <GL/glew.h>
#endif
#ifdef __APPLE__
#    define GLFW_INCLUDE_GLCOREARB
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>
#include "demo.h"
#include <sb.h>
#include <assert.h>
#include <gui/state.h>
#include <gui/grid.h>
#include <gui/menu.h>
#include <gui/component.h>
#include "sb_machine.h"

static struct sb_context context;
static struct ufsm_machine *m;
static struct component *root_view;
static struct component *last;
static struct component *selected = NULL;
static double ox,oy;
static GLFWwindow* window;

void errorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    NVG_NOTUSED(scancode);
    NVG_NOTUSED(mods);

    //printf ("Key event: %d %d %d\n",key, action, mods);

    if (key == GLFW_KEY_A && action == 1)
        ufsm_process(m, EV_KEY_A);
    if (key == GLFW_KEY_S && action == 1)
        ufsm_process(m, EV_KEY_S);
    if (key == GLFW_KEY_ENTER && action == 1)
        ufsm_process(m, EV_ENTER);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //printf("Scroll event: %f %f\n",xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    //printf("Mouse event: %i %i %i\n",button,action,mods);

    if (button == 0 && action == 1)
        ufsm_process(m, EV_LEFT_CLICK);

    if (button == 0 && action == 0)
        ufsm_process(m, EV_LEFT_CLICK_UP);
}

static void cursor_position_callback(GLFWwindow* window,
                                     double xpos,
                                     double ypos)
{
    ufsm_process(m, EV_CURSOR);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    printf("Window resize event: %u %u\n",width, height);
}


void select_component(void)
{
    double mx,my;
    glfwGetCursorPos(window, &mx, &my);
    struct component *selected_last = NULL;

    selected = NULL;


    for (struct component *c = root_view;c;c = c->next)
    {
        if ( (mx > c->x && mx < (c->x+c->w)) &&
             (my > c->y && my < (c->y+c->h)))
        {
            selected = c;
            selected_last = c;
        }
        else
        {
            c->selected = false;
        }
    }

    if (selected_last)
    {
        selected_last->selected = true;
        ufsm_process(m, EV_SELECTED);
    }
}

void select_move_component(void)
{
    double mx,my;
    glfwGetCursorPos(window, &mx, &my);
    struct component *selected_last = NULL;

    selected = NULL;


    for (struct component *c = root_view;c;c = c->next)
    {
        if ( (mx > c->x && mx < (c->x+c->w)) &&
             (my > c->y && my < (c->y+c->h)))
        {
            selected = c;
            selected_last = c;
        }
        else
        {
            c->selected = false;
        }
    }

    if (selected_last)
    {
        selected_last->selected = true;
        ufsm_process(m, EV_MOVE);
    }
}

void save_offset(void)
{
    double mx,my;
    glfwGetCursorPos(window, &mx, &my);
    ox = (int) ((selected->x - mx)/20)*20;
    oy = (int) ((selected->y - my)/20)*20;
}

void move_component(void)
{
    double mx,my;
    glfwGetCursorPos(window, &mx, &my);
    selected->x = (int) ((mx+ox)/20)*20;
    selected->y = (int) ((my+oy)/20)*20;
}

void check_hot_zones(void)
{
    //ufsm_process(m,EV_MOVE);

    double mx,my;
    glfwGetCursorPos(window, &mx, &my);
    component_hotzone_selected(selected, mx, my);
}

void resize_component(void)
{

}

void finalize_empty_state(void)
{
    struct component *c = last->next;
    c->alpha = 1.0;
    last = c;
    ufsm_process(m,EV_COMPLETED);
}

void hide_add_menu(void)
{
}

void show_add_menu(void)
{
}

void start_add_menu_timer(void)
{
}

void stop_add_menu_timer(void)
{
}

void create_empty_state(void)
{
    double my,mx;

    state_create(&last->next, "New state");
    struct component *c = last->next;
    glfwGetCursorPos(window, &mx, &my);
    c->x = mx;
    c->y = my;
    c->w = 200;
    c->h = 200;
    c->alpha = 0.3;
}

void update_empty_state1(void)
{
    double my,mx;
    struct component *c = last->next;
    glfwGetCursorPos(window, &mx, &my);
    c->x = (int) (mx/20)*20;
    c->y = (int) (my/20)*20;
}

void update_empty_state2(void)
{
    double my,mx;
    struct component *c = last->next;
    glfwGetCursorPos(window, &mx, &my);
    c->w = (int) ((mx-c->x)/20)*20;
    c->h = (int) ((my-c->y)/20)*20;
}

int main(int argc, char **argv)
{
    char banner[128];
    NVGcontext* vg = NULL;

    if (!glfwInit())
    {
        printf("Failed to init GLFW.");
        return -1;
    }

    snprintf(banner, sizeof(banner), "State blaster v%s", PACKAGE_VERSION);

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
        return SB_ERR;
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
        return SB_ERR;
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

    glfwSwapInterval(0);

    grid_create(&root_view);
    grid_set_spacing(root_view, 20);
    last = root_view;

    m = get_MainMachine();

    ufsm_debug_machine(m);
    ufsm_init_machine(m);
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

        for (struct component *c = root_view; c; c = c->next)
            component_render(vg, c, winWidth, winHeight);

        nvgEndFrame(vg);
        glfwSwapBuffers(window);
        glfwWaitEventsTimeout(0.5);

        struct ufsm_queue *q = ufsm_get_queue(m);
        uint32_t q_ev;
        uint32_t err;

        /* Process queued events */
        while (ufsm_queue_get(q, &q_ev) == UFSM_OK)
        {
            err = ufsm_process(m, q_ev);

            if (err != UFSM_OK)
                printf("ERROR: %s\n", ufsm_errors[err]);
            assert(err == UFSM_OK);


            if (m->stack.pos == UFSM_STACK_SIZE)
                printf("ERROR: Stack overflow!\n");
            else if (m->stack.pos > 0)
                printf("ERROR: Stack did not return to zero\n");
            assert (m->stack.pos == 0);
        }

    }

    nvgDeleteGL3(vg);
    glfwTerminate();

    return SB_OK;
}
