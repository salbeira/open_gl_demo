#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <GL/gl.h>

GLfloat length(GLfloat *vector);
GLfloat dot3x3(GLfloat *a, GLfloat *b);
void normalize(GLfloat *vector);
void normal(GLfloat *normal, GLfloat *vector);
void cross(GLfloat *result, GLfloat *a, GLfloat *b);

#endif
