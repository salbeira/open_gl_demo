#include "snow_demo.h"

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
/* malloc, srand, rand */
#include <stdlib.h>
/* printf */
#include <stdio.h>
/* time */
#include <time.h>

#define MATRIX_SIZE (16 * sizeof(GLfloat))
#define SNOW_AMOUNT 128

GLuint points_vertex_array;
GLuint points_index_buffer;

GLuint snow_program;
GLuint snow_program_matrices_buffer;
GLuint snow_program_level_buffer;

GLuint snow_position_buffer;

GLfloat snow_camera[9];
GLfloat snow_position[SNOW_AMOUNT * 3];
GLfloat snow_fall[SNOW_AMOUNT];

GLfloat snow_texture;

GLint inner_snow_level = 1;
GLint outer_snow_level = 1;

void init_snow_program()
{
	/* Create Shader */
	GLuint vs = compile_shader("shader/snow_vertex_shader.glsl", GL_VERTEX_SHADER);
	GLuint gs = compile_shader("shader/snow_geometry_shader.glsl", GL_GEOMETRY_SHADER);
	GLuint fs = compile_shader("shader/snow_fragment_shader.glsl", GL_FRAGMENT_SHADER);

	if(vs == 0) return;
	if(gs == 0) return;
	if(fs == 0) return;

	snow_program = glCreateProgram();
	glAttachShader(snow_program, vs);
	glAttachShader(snow_program, gs);
	glAttachShader(snow_program, fs);
	glLinkProgram(snow_program);

	/* Check success of linking */
	GLint success;
	glGetProgramiv(snow_program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(snow_program, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = (GLchar *) malloc(log_length * sizeof(GLchar));
		glGetProgramInfoLog(snow_program, log_length, &log_length, log);
		printf("[ERROR] glLinkShader for snow_program: \n%s\n)", log);
		free(log);
		glDeleteProgram(snow_program);
		exit(128);
	}

	printf("[INFO] Snow Demo program prepared\n");

	/* Set up uniform buffer objects for the program */
	
	glGenBuffers(1, &snow_program_matrices_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, snow_program_matrices_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16 * 3, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	/* Create matrices on the stack */	

	/* Create Projection Matrix */
	GLfloat projection_matrix[16];
	fill_projection(projection_matrix, 45.0f, 8.0f/6.0f, 0.1f, 100.0f);		

	/* Where the camera is at */
	snow_camera[0] = 10.0f;
	snow_camera[1] = 10.0f;
	snow_camera[2] = 10.0f;

	/* Where the camera looks at */
	snow_camera[3] = 0.0;
	snow_camera[4] = 0.0;
	snow_camera[5] = 0.0;

	/* The up vector of the camera */
	snow_camera[6] = 0.0;
	snow_camera[7] = 1.0;
	snow_camera[8] = 0.0;

	/* Create View Matrix */
	GLfloat view_matrix[16];
	fill_look_at(view_matrix, snow_camera, snow_camera + 3, snow_camera + 6); 

	/* Create Model Matrix */
	GLfloat model_matrix[16];
	fill_identity(model_matrix);

	/* Copy data from the stack to the GPU */

	glBufferSubData(GL_UNIFORM_BUFFER, 0, MATRIX_SIZE, projection_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE*2, MATRIX_SIZE, model_matrix); 

	GLuint snow_matrices_index = glGetUniformBlockIndex(snow_program, "Matrices");
	glUniformBlockBinding(snow_program, snow_matrices_index, 3);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, snow_program_matrices_buffer); 

	printf("[INFO] Snow Demo matrices prepared\n");


}

void init_points_data()
{
	GLubyte points_indices[SNOW_AMOUNT];

	srand(time(NULL));
	for(int i = 0; i < SNOW_AMOUNT; i++)
	{
		GLfloat x = (SNOW_AMOUNT / 2.0 - (rand() % SNOW_AMOUNT)) / 4.0;
		GLfloat y = ((GLfloat)(rand() % SNOW_AMOUNT)) / 16.0;
		GLfloat z = (SNOW_AMOUNT / 2.0 - (rand() % SNOW_AMOUNT)) / 4.0;
		snow_position[3 * i] = x;
		snow_position[3 * i + 1] = y;
		snow_position[3 * i + 2] = z;
		GLfloat fall = ((GLfloat) (rand() % SNOW_AMOUNT)) / 32.0;
		snow_fall[i] = fall > 0.5 ? fall : 0.5;
		points_indices[i] = i;
	}
	
	/* Create objects and fill buffers with data */	

	glGenVertexArrays(1, &points_vertex_array);
	glGenBuffers(1, &snow_position_buffer);
	glGenBuffers(1, &points_index_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, snow_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snow_position), snow_position, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, points_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(points_indices), points_indices, GL_STATIC_DRAW);

	glBindVertexArray(points_vertex_array);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, snow_position_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	/* Create texture */

	GLvoid *texture_data;
	GLuint  texture_width;
	GLuint  texture_height;
	read_png_data(&texture_data, &texture_width, &texture_height, "image/snow_texture.png");		
	
	snow_texture = create_texture(texture_width, texture_height, texture_data);

	printf("[INFO] Snow Demo objects prepared\n");
}

void render_snow_demo()
{
	glUseProgram(snow_program);
	glBindVertexArray(points_vertex_array);
	glBindTexture(snow_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, points_index_buffer);

	glDrawElements(GL_POINTS, SNOW_AMOUNT, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_snow_demo(double delta)
{
	update_snow_camera(delta);

	/* Update uniform buffers */

	GLfloat view_matrix[16];
	fill_look_at(view_matrix, snow_camera, snow_camera + 3, snow_camera + 6);

	for(int i = 0; i < SNOW_AMOUNT; i++)
	{
		GLfloat y = snow_position[i * 3 + 1];
		y -= snow_fall[i] * delta;
		if(y < 0)
			y = ((GLfloat)(rand() % SNOW_AMOUNT)) / 16.0;
		snow_position[i * 3 + 1] = y;
	}

	glBindBuffer(GL_ARRAY_BUFFER, snow_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snow_position), snow_position, GL_STATIC_DRAW);


	glBindBuffer(GL_UNIFORM_BUFFER, snow_program_matrices_buffer); 
	glBufferSubData(GL_UNIFORM_BUFFER, MATRIX_SIZE, MATRIX_SIZE, view_matrix);
}

void update_snow_camera(double delta)
{
	GLfloat factor = 10.0f * (float) delta;	

	GLfloat *eye = snow_camera;
	GLfloat *at = snow_camera + 3;
	GLfloat *up = snow_camera + 6;

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

void print_snow_demo_help()
{
	printf("[HELP] Active Demo: Snow Demo!\n");
	printf("[HELP] Use the buttons A and D or LEFT and RIGHT to move the camera around the object horizontally.\n");
	printf("[HELP] Use the buttons W and S or UP and DOWN to zoom in and out from the object.\n");
	printf("[HELP] Use the buttons Q and E to move the camera around the object vertically.\n");
	printf("[HELP] To move the camera, you can also use the UP, DOWN, LEFT and RIGHT button on your keyboard.\n");
}
