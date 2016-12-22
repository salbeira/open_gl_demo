#include "init_window.h"
#include "globals.h"
#include "keyboard.h"
#include "mouse.h"

#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

void init_window()
{
	if(!glfwInit()) exit(64);
	window = glfwCreateWindow(800,600, "OpenGL Demonstration", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(64);
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, &key_callback);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
	glfwMakeContextCurrent(window);
}
