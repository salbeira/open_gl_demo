#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>

#include <stdlib.h>

#include "main.h"
#include "globals.h"
#include "init.h"
#include "render.h"
#include "update.h"
#include "load.h"

#include "cube_demo.h"

unsigned int fps = 0;

int main(int argc, char** argv)
{
	init();
	printf("[HELP] Press F1 - F4 to switch between demos.\n");
	print_cube_demo_help();
	double lastTime 	= glfwGetTime();
	double updateTime 	= glfwGetTime();

	unsigned int local_fps;

	while(!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		local_fps++;
		if(currentTime - lastTime >= 1.0)
		{
			fps = local_fps;
			local_fps = 0;
			lastTime += 1.0;
		}
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		update(currentTime - updateTime);
		updateTime = currentTime;
	}
	glfwTerminate();
	return 0;
}
