#include "init.h"
#include "init_window.h"
#include "init_gl.h"

#include "cube_demo.h"
#include "tesselation_demo.h"
#include "snow_demo.h"

void init()
{
	init_window();
	init_gl();
	init_default_program();
	init_cube_data();
	init_tesselation_program();
	init_icosahedron_data();
	init_snow_program();
	init_points_data();
}
