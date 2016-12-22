#include "cube_demo.h"

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

GLuint cube_vertex_array;
GLuint cube_index_buffer;

GLuint default_program;
GLuint default_program_matrices_buffer;

GLfloat cube_camera[9];

GLuint cube_texture;

void init_default_program()
{
	/* Create Shader */
	GLuint vs = compile_shader("shader/default_vertex_shader.glsl", GL_VERTEX_SHADER);
	GLuint fs = compile_shader("shader/default_fragment_shader.glsl", GL_FRAGMENT_SHADER);

	if(vs == 0) return;
	if(fs == 0) return;

	default_program = glCreateProgram();
	glAttachShader(default_program, vs);
	glAttachShader(default_program, fs);
	glLinkProgram(default_program);

	/* Check success of linking */
	GLint success;
	glGetProgramiv(default_program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(default_program, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = (GLchar *) malloc(log_length * sizeof(GLchar));
		glGetProgramInfoLog(default_program, log_length, &log_length, log);
		printf("[ERROR] glLinkShader for default_program: \n%s\n)", log);
		free(log);
		glDeleteProgram(default_program);
		exit(128);
	}

	printf("[INFO] Cube Demo program prepared\n");

	/* Set up uniform buffer objects for the program */
	
	glGenBuffers(1, &default_program_matrices_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, default_program_matrices_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16 * 3, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	/* Create matrices on the stack */	

	/* Create Projection Matrix */
	GLfloat projection_matrix[16];
	fill_projection(projection_matrix, 45.0f, 8.0f/6.0f, 0.1f, 100.0f);		

	/* Where the camera is at */
	cube_camera[0] = 10.0f;
	cube_camera[1] = 10.0f;
	cube_camera[2] = 10.0f;

	/* Where the camera looks at */
	cube_camera[3] = 0.0;
	cube_camera[4] = 0.0;
	cube_camera[5] = 0.0;

	/* The up vector of the camera */
	cube_camera[6] = 0.0;
	cube_camera[7] = 1.0;
	cube_camera[8] = 0.0;

	/* Create View Matrix */
	GLfloat view_matrix[16];
	fill_look_at(view_matrix, cube_camera, cube_camera + 3, cube_camera + 6); 

	/* Create Model Matrix */
	GLfloat model_matrix[16];
	fill_identity(model_matrix);

	/* Copy data from the stack to the GPU */

	glBufferSubData(GL_UNIFORM_BUFFER, 0, MATRIX_SIZE, projection_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE*2, MATRIX_SIZE, model_matrix); 

	GLuint default_uniform_index = glGetUniformBlockIndex(default_program, "Matrices");
	glUniformBlockBinding(default_program, default_uniform_index, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, default_program_matrices_buffer); 

	printf("[INFO] Cube Demo matrices prepared\n");
}

void init_cube_data()
{
	GLfloat cube_vertices[] =
	{
		-1,-1,-1,	-1, 1,-1,	1,-1,-1,	1, 1,-1, 	//Front
		 1,-1, 1,	1, 1, 1,	-1,-1, 1,	-1, 1, 1,	//Back
		 1,-1,-1,	1, 1,-1,	1,-1, 1,	1, 1, 1,	//Right
		-1,-1, 1,	-1, 1, 1,	-1,-1,-1,	-1, 1,-1,	//Left
		-1, 1,-1,	-1, 1, 1,	1, 1,-1,	1, 1, 1,	//Top
		-1,-1, 1,	-1,-1,-1,	1,-1, 1,	1,-1,-1		//Bottom
	};
	GLfloat cube_texture_coordinates[] =
	{
		0,0, 0,1, 1,0, 1,1,	//Front
		0,0, 0,1, 1,0, 1,1,	//Back
		0,0, 0,1, 1,0, 1,1,	//Right
		0,0, 0,1, 1,0, 1,1,	//Left
		0,0, 0,1, 1,0, 1,1,	//Top
		0,0, 0,1, 1,0, 1,1	//Bottom
	};
	GLubyte cube_indices[] =
	{
		 0, 1, 2,	 2, 1, 3,	//Front
		 4, 5, 6,	 6, 5, 7,	//Right
		 8, 9,10,	10, 9,11,	//Left
		12,13,14,	14,13,15,	//Back
		16,17,18,	18,17,19,	//Top
		20,21,22,	22,21,23	//Bottom
	};
	
	/* Create objects and fill buffers with data */

	GLuint cube_vertices_buffer;
	GLuint cube_texture_coordinate_buffer;

	glGenVertexArrays(1, &cube_vertex_array);
	glGenBuffers(1, &cube_vertices_buffer);
	glGenBuffers(1, &cube_texture_coordinate_buffer);
	glGenBuffers(1, &cube_index_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vertices_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cube_texture_coordinate_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texture_coordinates), cube_texture_coordinates, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glBindVertexArray(cube_vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vertices_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, cube_texture_coordinate_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	/* Create texture */

	GLvoid *texture_data;
	GLuint  texture_width;
	GLuint  texture_height;
	read_png_data(&texture_data, &texture_width, &texture_height, "image/cube_texture.png");		
	
	cube_texture = create_texture(texture_width, texture_height, texture_data);

	printf("[INFO] Cube Demo objects prepared\n");
}

void render_cube_demo()
{
	glUseProgram(default_program);
	glBindTexture(GL_TEXTURE_2D, cube_texture);
	glBindVertexArray(cube_vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_buffer);
	
	glDrawElements(GL_TRIANGLES, 6*6*4, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_cube_demo(double delta)
{
	update_cube_camera(delta);

	/* Update uniform buffers */

	GLfloat view_matrix[16];
	fill_look_at(view_matrix, cube_camera, cube_camera + 3, cube_camera + 6);
	glBindBuffer(GL_UNIFORM_BUFFER, default_program_matrices_buffer); 
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
	
}

void update_cube_camera(double delta)
{
	GLfloat factor = 10.0f * (float) delta;	

	GLfloat *eye = cube_camera;
	GLfloat *at = cube_camera + 3;
	GLfloat *up = cube_camera + 6;

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

void print_cube_demo_help()
{
	printf("[HELP] Active Demo: Cube Demo!\n");
	printf("[HELP] Use the buttons A and D or LEFT and RIGHT to move the camera around the cube horizontally.\n");
	printf("[HELP] Use the buttons W and S or UP and DOWN to zoom in and out from the cube.\n");
	printf("[HELP] Use the buttons Q and E to move the camera around the cube vertically.\n");
	printf("[HELP] Tilt the camera height with Q and E.\n");
	printf("[HELP] To move the camera, you can also use the UP, DOWN, LEFT and RIGHT button on your keyboard.\n");
}
