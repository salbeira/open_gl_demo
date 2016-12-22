#ifndef _LOAD_H_
#define _LOAD_H_

/* GLtype */
#include <GL/gl.h> 

char *read_file(const char *filename);

void read_file_data(size_t *size, char **buffer, const char *filename);

void read_png_data(GLvoid **data, GLuint *width, GLuint *height, const char *filename);

GLuint compile_shader(const char *filename, GLenum type);

#endif
