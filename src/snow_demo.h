#ifndef _SNOW_DEMO_H_
#define _SNOW_DEMO_H_

#include <GL/gl.h>

/* Names of GL objects for a snow demo */
extern GLuint points_vertex_array;
extern GLuint points_index_buffer;

extern GLfloat snow_camera[];
extern GLfloat snow_position[];

extern GLuint snow_program;
extern GLuint snow_program_matrices_buffer;
extern GLuint snow_program_level_buffer;

/* Functions */

void init_snow_program();
void init_points_data();

void render_snow_demo();
void update_snow_demo(double delta);
void update_snow_camera(double delta);

void print_snow_demo_help();

#endif
