#include "tesselation_demo.h"

/* read_file_data, read_png_data */
#include "load.h"
#include "texture.h"
/* projection, view and model */
#include "matrix.h"
#include "vector.h"

/* key values */
#include "keyboard.h"

/* strlen */
#include <string.h>
/* OpenGL */
#include <GL/gl.h>
/* malloc */
#include <stdlib.h>
/* printf */
#include <stdio.h>

#define MATRIX_SIZE (16 * sizeof(GLfloat))

GLuint icosahedron_vertex_array;
GLuint icosahedron_index_buffer;

GLuint tesselation_program;
GLuint tesselation_program_matrices_buffer;
GLuint tesselation_program_level_buffer;

GLfloat tesselation_camera[9];

GLint inner_tesselation_level = 1;
GLint outer_tesselation_level = 1;

void init_tesselation_program()
{
	/* Create Shader */
	GLuint vs = compile_shader("shader/tesselation_vertex_shader.glsl", GL_VERTEX_SHADER);
	GLuint tcs = compile_shader("shader/tesselation_control_shader.glsl", GL_TESS_CONTROL_SHADER);	
	GLuint tes = compile_shader("shader/tesselation_evaluation_shader.glsl", GL_TESS_EVALUATION_SHADER);
	GLuint gs = compile_shader("shader/tesselation_geometry_shader.glsl", GL_GEOMETRY_SHADER);
	GLuint fs = compile_shader("shader/tesselation_fragment_shader.glsl", GL_FRAGMENT_SHADER);

	if(vs == 0) return;
	if(tcs == 0) return;
	if(tes == 0) return;
	if(gs == 0) return;
	if(fs == 0) return;

	tesselation_program = glCreateProgram();
	glAttachShader(tesselation_program, vs);
	glAttachShader(tesselation_program, tcs);
	glAttachShader(tesselation_program, tes);
	glAttachShader(tesselation_program, gs);
	glAttachShader(tesselation_program, fs);
	glLinkProgram(tesselation_program);

	/* Check success of linking */
	GLint success;
	glGetProgramiv(tesselation_program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(tesselation_program, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = (GLchar *) malloc(log_length * sizeof(GLchar));
		glGetProgramInfoLog(tesselation_program, log_length, &log_length, log);
		printf("[ERROR] glLinkShader for tesselation_program: \n%s\n)", log);
		free(log);
		glDeleteProgram(tesselation_program);
		exit(128);
	}

	printf("[INFO] Tesselation Demo program prepared\n");

	/* Set up uniform buffer objects for the program */
	
	glGenBuffers(1, &tesselation_program_matrices_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, tesselation_program_matrices_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16 * 3, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	/* Create matrices on the stack */	

	/* Create Projection Matrix */
	GLfloat projection_matrix[16];
	fill_projection(projection_matrix, 45.0f, 8.0f/6.0f, 0.1f, 100.0f);		

	/* Where the camera is at */
	tesselation_camera[0] = 10.0f;
	tesselation_camera[1] = 10.0f;
	tesselation_camera[2] = 10.0f;

	/* Where the camera looks at */
	tesselation_camera[3] = 0.0;
	tesselation_camera[4] = 0.0;
	tesselation_camera[5] = 0.0;

	/* The up vector of the camera */
	tesselation_camera[6] = 0.0;
	tesselation_camera[7] = 1.0;
	tesselation_camera[8] = 0.0;

	/* Create View Matrix */
	GLfloat view_matrix[16];
	fill_look_at(view_matrix, tesselation_camera, tesselation_camera + 3, tesselation_camera + 6); 

	/* Create Model Matrix */
	GLfloat model_matrix[16];
	fill_identity(model_matrix);

	/* Copy data from the stack to the GPU */

	glBufferSubData(GL_UNIFORM_BUFFER, 0, MATRIX_SIZE, projection_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE*2, MATRIX_SIZE, model_matrix); 

	GLuint tesselation_matrices_index = glGetUniformBlockIndex(tesselation_program, "Matrices");
	glUniformBlockBinding(tesselation_program, tesselation_matrices_index, 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, tesselation_program_matrices_buffer); 

	/* Tesselation Level Uniform Data */
	glGenBuffers(1, &tesselation_program_level_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, tesselation_program_level_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(GLint) * 2, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	GLint level[2] = {inner_tesselation_level, outer_tesselation_level};
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLint) * 2, level);		

	GLuint tesselation_level_index = glGetUniformBlockIndex(tesselation_program, "Tesselation");
	glUniformBlockBinding(tesselation_program, tesselation_level_index, 2);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, tesselation_program_level_buffer);

	printf("[INFO] Tesselation Demo matrices prepared\n");
}

void init_icosahedron_data()
{
	GLfloat icosahedron_vertices[] =
	{
		 0.000f,  0.000f,  1.000f,
		 0.894f,  0.000f,  0.447f,
		 0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		 0.276f, -0.851f,  0.447f,
		 0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		 0.724f, -0.526f, -0.447f,
		 0.000f,  0.000f, -1.000f
	};
	GLubyte icosahedron_indices[] =
	{
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,
		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,
		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,
		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10
	};
	
	/* Create objects and fill buffers with data */

	GLuint icosahedron_vertices_buffer;

	glGenVertexArrays(1, &icosahedron_vertex_array);
	glGenBuffers(1, &icosahedron_vertices_buffer);
	glGenBuffers(1, &icosahedron_index_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, icosahedron_vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(icosahedron_vertices), icosahedron_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedron_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icosahedron_indices), icosahedron_indices, GL_STATIC_DRAW);

	glBindVertexArray(icosahedron_vertex_array);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, icosahedron_vertices_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	printf("[INFO] Tesselation Demo objects prepared\n");
}

void render_tesselation_demo()
{
	glUseProgram(tesselation_program);
	glBindVertexArray(icosahedron_vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedron_index_buffer);
	
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	glDrawElements(GL_PATCHES, 3 * 20 , GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_tesselation_demo(double delta)
{
	update_tesselation_camera(delta);

	/* Update uniform buffers */

	GLfloat view_matrix[16];
	fill_look_at(view_matrix, tesselation_camera, tesselation_camera + 3, tesselation_camera + 6);
	glBindBuffer(GL_UNIFORM_BUFFER, tesselation_program_matrices_buffer); 
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
	
	GLint level[2] = {inner_tesselation_level, outer_tesselation_level};
	glBindBuffer(GL_UNIFORM_BUFFER, tesselation_program_level_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLint) * 2, level);	
}

void update_tesselation_camera(double delta)
{
	GLfloat factor = 10.0f * (float) delta;	

	GLfloat *eye = tesselation_camera;
	GLfloat *at = tesselation_camera + 3;
	GLfloat *up = tesselation_camera + 6;

	GLfloat z[3] = { (at[0] - eye[0]),
					 (at[1] - eye[1]),
					 (at[2] - eye[2])};
	
	normalize(z);
	GLfloat x[3];
	cross(x, z, up);
	normalize(x);
	GLfloat y[3];
	cross(y, z, x);
	normalize(y);

	char mz = (key_up - key_down);
	char mx = (key_right - key_left);
	char my = (key_low - key_high);

	eye[0] = eye[0] + factor * (x[0] * mx + y[0] * my + z[0] * mz);
	eye[1] = eye[1] + factor * (x[1] * mx + y[1] * my + z[1] * mz);
	eye[2] = eye[2] + factor * (x[2] * mx + y[2] * my + z[2] * mz);
}

void print_tesselation_demo_help()
{
	printf("[HELP] Active Demo: Tesselation Demo!\n");
	printf("[HELP] Use the buttons A and D or LEFT and RIGHT to move the camera around the object horizontally.\n");
	printf("[HELP] Use the buttons W and S or UP and DOWN to zoom in and out from the object.\n");
	printf("[HELP] Use the buttons Q and E to move the camera around the object vertically.\n");
	printf("[HELP] Tilt the camera height with Q and E.\n");
	printf("[HELP] To move the camera, you can also use the UP, DOWN, LEFT and RIGHT button on your keyboard.\n");
	printf("[HELP] Use the I and O button to change the inner and outer tesselation level used in the shader.\n");
}
