#include "init_gl.h"

#include "globals.h"
	
void init_gl()
{
	/* Set the viewport to the window's width and height */
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width, height);
	/* Set the clear color */
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	/*
	 * Enable face culling (not rendering faces facing away from the camera)
     * Enable blending (using the alpha channel to render transparent fragments)
     * Enable depth testing (not rendering objects that are behind objects already rendered
     */
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
