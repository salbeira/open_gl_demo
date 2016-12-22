#ifndef _TESSELATION_DEMO_H_
#define _TESSELATION_DEMO_H_

#include <GL/gl.h>

/* Names of GL objects for a tesselation demo */
extern GLuint icosahedron_vertex_array;
extern GLuint icosahedron_index_buffer;

extern GLfloat tesselation_camera[];

extern GLuint tesselation_program;
extern GLuint tesselation_program_matrices_buffer;
extern GLuint tesselation_program_level_buffer;

extern GLint inner_tesselation_level;
extern GLint outer_tesselation_level;

/* Functions */

void init_tesselation_program();
void init_icosahedron_data();

void render_tesselation_demo();
void update_tesselation_demo(double delta);
void update_tesselation_camera(double delta);

void print_tesselation_demo_help();

#endif
