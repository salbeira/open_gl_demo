#ifndef _CUBE_DEMO_H_
#define _CUBE_DEMO_H_

#include <GL/gl.h>

/* Names of GL objects for a basic cube demo */
extern GLuint cube_vertex_array;
extern GLuint cube_index_buffer;

extern GLfloat cube_camera[];

extern GLuint default_program;
extern GLuint default_program_uniform_buffer;

extern GLuint cube_texture;

/* Functions */

void init_default_program();
void init_cube_data();

void render_cube_demo();
void update_cube_demo(double delta);
void update_cube_camera(double delta);

void print_cube_demo_help();

#endif
