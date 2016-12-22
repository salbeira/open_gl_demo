#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "vector.h"

/* Matrices in OpenGL are represented column by column, not row by row ... */

void add(GLfloat *result, GLfloat *a, GLfloat *b);
void multiply(GLfloat *result, GLfloat *a, GLfloat *b);

void get_column(GLfloat *result, GLfloat *matrix, unsigned int column);
void get_row(GLfloat *result, GLfloat *matrix, unsigned int row);

void fill_look_at(GLfloat *result, GLfloat *exe, GLfloat *at, GLfloat *up);
void fill_projection(GLfloat *result, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

void fill_identity(GLfloat *result);
void fill_translation(GLfloat *result, GLfloat *vector);

#endif
