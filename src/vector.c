#include "vector.h"

#include <math.h>

GLfloat length(GLfloat *vector)
{
	return sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
}

GLfloat dot3x3(GLfloat *a, GLfloat *b)
{
	GLfloat result = 0.0f;
	for(unsigned int i = 0; i < 3; i++)
	{
		result += a[i]*b[i];
	}
	return result;
}

void normalize(GLfloat *vector)
{
	GLfloat len = length(vector);
	vector[0] = vector[0] / len;
	vector[1] = vector[1] / len;
	vector[2] = vector[2] / len;
}

void normal(GLfloat *normal, GLfloat *vector)
{
	GLfloat len = length(vector);
	normal[0] = vector[0] / len;
	normal[1] = vector[1] / len;
	normal[2] = vector[2] / len;
}

void cross(GLfloat *result, GLfloat *a, GLfloat *b)
{
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
}
