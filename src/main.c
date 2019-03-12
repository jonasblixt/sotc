#include <stdio.h>
#ifdef NANOVG_GLEW
#	include <GL/glew.h>
#endif
#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>
#include "demo.h"
#include <sb.h>
#include <gui/state.h>
#include <gui/grid.h>
static struct sb_context context;

void errorcb(int error, const char* desc)
{
	printf("GLFW error %d: %s\n", error, desc);
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);

	printf ("Key event: %d %d %d\n",key, action, mods);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    printf("Scroll event: %f %f\n",xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    printf("Mouse event: %i %i %i\n",button,action,mods);
}

static void cursor_position_callback(GLFWwindow* window, 
                                     double xpos, 
                                     double ypos)
{
    printf ("Cursor event: %f %f\n",xpos,ypos);
}

int main(int argc, char **argv)
{
	GLFWwindow* window;
	DemoData data;
	NVGcontext* vg = NULL;

	if (!glfwInit()) 
	{
		printf("Failed to init GLFW.");
		return -1;
	}


	glfwSetErrorCallback(errorcb);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	window = glfwCreateWindow(1000, 600, "State Blaster", NULL, NULL);

	if (!window) 
	{
		glfwTerminate();
		return SB_ERR;
	}

	glfwSetKeyCallback(window, key);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
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
/**
 *
 * component
 * component
 *      component
 *          component
 *      component
 *
 * x,y,z,w,h
 * type
 *
 *
 */
        struct component *c;
        
        grid_create(&c);


        grid_render(vg,c);
        struct sb_state s;
        s.x = 100;
        s.y = 100;
        s.w = 200;
        s.h = 300;
        s.name = malloc(255);
        sprintf(s.name,"Test state2");

        render_state(vg, &s);
		nvgEndFrame(vg);
		glfwSwapBuffers(window);
        glfwWaitEventsTimeout(0.5);
	}

	nvgDeleteGL3(vg);
	glfwTerminate();

	return SB_OK;
}
