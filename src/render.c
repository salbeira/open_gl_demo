#include "render.h"

#include <GL/gl.h>

#include "matrix.h"
#include "globals.h"

#include "cube_demo.h"
#include "tesselation_demo.h"
#include "snow_demo.h"

unsigned char polygon_mode = 0;
unsigned int active_demo = 1;

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, polygon_mode ? GL_LINE : GL_FILL);

	switch(active_demo)
	{
		case 1:
			render_cube_demo();
			break;
		case 2:
			render_snow_demo();
			break;
		case 4:
			render_tesselation_demo();
			break;
	}		
	
}
