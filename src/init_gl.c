#include "init_gl.h"

#include "globals.h"
	
void init_gl()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width, height);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
