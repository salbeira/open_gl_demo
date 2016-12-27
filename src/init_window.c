#include "init_window.h"
#include "globals.h"
#include "keyboard.h"
#include "mouse.h"

#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

/* Basic glfw window initialization */
void init_window()
{
	/* Init the companion API and create a new window */
	if(!glfwInit())
	{
		exit(64);
	}
	window = glfwCreateWindow(800,600, "OpenGL Demonstration", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(64);
	}
	/* Activate VSync if possible */
	glfwSwapInterval(1);

	/* Keyboard and Mouse callback functions */
	glfwSetKeyCallback(window, &key_callback);
	glfwSetCursorPosCallback(window, &cursor_position_callback);

	/* Make the created OpenGL context of the window current in the running thread */
	glfwMakeContextCurrent(window);
}
