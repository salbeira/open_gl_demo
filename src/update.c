#include "update.h"

#include "globals.h"
#include "cube_demo.h"
#include "tesselation_demo.h"
#include "snow_demo.h"

/* Update the active demo */
void update(double delta)
{
	switch(active_demo)
	{
		case 1:
			update_cube_demo(delta);
			break;
		case 2:
			update_snow_demo(delta);
			break;
		case 4:
			update_tesselation_demo(delta);
			break;
	}
}
