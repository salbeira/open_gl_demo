#include "mouse.h"

#include "globals.h"
#include "cube_demo.h"
#include "tesselation_demo.h"

#include "camera.h"

int last_x = 0;
int last_y = 0;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	int x = (int) xpos;
	int y = (int) ypos;
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS)
	{
		if(active_demo == 1)
		{
			move(cube_camera, x - last_x, y - last_y, 0 ); 
		}	
	}
	last_x = (int) x;
	last_y = (int) y;
}
