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
#include <sb/sb.h>

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

int main()
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
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);


	window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);

	if (!window) 
	{
		glfwTerminate();
		return SB_ERR;
	}

	glfwSetKeyCallback(window, key);
	glfwMakeContextCurrent(window);

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	if (vg == NULL) 
	{
		printf("Could not init nanovg.\n");
		return SB_ERR;
	}

	if (loadDemoData(vg, &data) == -1)
		return SB_ERR;

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
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
		renderDemo(vg, mx,my, winWidth,winHeight, t, 0, &data);
		nvgEndFrame(vg);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeDemoData(vg, &data);
	nvgDeleteGL3(vg);
	glfwTerminate();

	return SB_OK;
}
