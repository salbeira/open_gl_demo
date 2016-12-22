#include "keyboard.h"

#include "render.h"

#include "cube_demo.h"
#include "tesselation_demo.h"

#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

unsigned char key_up = 0;
unsigned char key_down = 0;
unsigned char key_left = 0;
unsigned char key_right = 0;
unsigned char key_high = 0;
unsigned char key_low = 0;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	/* Key for camera movement */
	if((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
		key_up = 1;
	else if((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_RELEASE)
		key_up = 0;
	
	else if((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
		key_down = 1;
	else if((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_RELEASE)
		key_down = 0;

	else if((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
		key_left = 1;
	else if((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_RELEASE)
		key_left = 0;

	else if((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
		key_right = 1;
	else if((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_RELEASE)
		key_right = 0;

	else if((key == GLFW_KEY_Q) && action == GLFW_PRESS)
		key_high = 1;
	else if((key == GLFW_KEY_Q) && action == GLFW_RELEASE)
		key_high = 0;

	else if((key == GLFW_KEY_E) && action == GLFW_PRESS)
		key_low = 1;
	else if((key == GLFW_KEY_E) && action == GLFW_RELEASE)
		key_low = 0;

	/* Key for tesselation */

	else if((key == GLFW_KEY_I) && action == GLFW_PRESS){
		inner_tesselation_level = (inner_tesselation_level % 16) + 1;
		printf("[INFO] Changed inner tesselation level to: %d\n", inner_tesselation_level);
	}
	else if((key == GLFW_KEY_O) && action == GLFW_PRESS){
		outer_tesselation_level = (outer_tesselation_level % 16) + 1;
		printf("[INFO] Changed outer tesselation level to: %d\n", outer_tesselation_level);
	}

	/* Key for switching demos */

	else if((key == GLFW_KEY_F1) && action == GLFW_PRESS){
		active_demo = 1;
		print_cube_demo_help();
	}
	else if((key == GLFW_KEY_F4) && action == GLFW_PRESS){
		active_demo = 4;
		print_tesselation_demo_help();
	}

	/* Keys for configuration and information */

	else if(key == GLFW_KEY_P && action == GLFW_PRESS)
		polygon_mode = !polygon_mode;
	else if((key == GLFW_KEY_F) && action == GLFW_PRESS)
		printf("[INFO] FPS: %d\n", fps);
	else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}
