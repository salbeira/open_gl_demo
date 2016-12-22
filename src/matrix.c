#include "matrix.h"
#include "vector.h"

#include <math.h>

void add(GLfloat *result, GLfloat *a, GLfloat *b)
{
	for(int i = 0; i < 16; i++){
		result[i] = a[i] + b[i];
	}
}

void multiply(GLfloat *result, GLfloat *a, GLfloat *b)
{
	GLfloat sum;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			sum = 0;
			for(int k = 0; k < 4; k++) {
				sum += a[k * 4 + j] * b[i * 4 +k];
			}
			result[i * 4 + j] = sum;
		}
	}
}

void get_column(GLfloat *result, GLfloat *matrix, unsigned int column)
{
	if(column > 4) return;
	for(int i = 0; i < 4; i++)
	{
		result[i] = matrix[i + 4*column];
	}
}

void get_row(GLfloat *result, GLfloat *matrix, unsigned int row)
{
	if(row > 4) return;
	for(int i = 0; i < 4; i++)
	{
		result[i] = matrix[i * 4 + row];
	}
}

void fill_look_at(GLfloat *result, GLfloat *eye, GLfloat *at, GLfloat *up)
{
	GLfloat x[3],y[3],z[3] = {at[0] - eye[0], at[1] - eye[1], at[2] - eye[2]};
	normalize(z);
	cross(x, z, up);
	normalize(x);
	cross(y, x, z);
	result[ 0] = 		x[0]; result[ 1] = 			y[0]; result[ 2] =			-z[0]; result[ 3] = 0;
	result[ 4] = 		x[1]; result[ 5] = 			y[1]; result[ 6] =			-z[1]; result[ 7] = 0;
	result[ 8] = 		x[2]; result[ 9] = 			y[2]; result[10] =			-z[2]; result[11] = 0;
	result[12] = -dot3x3(x,eye); result[13] =  -dot3x3(y, eye); result[14] =   dot3x3(z, eye); result[15] = 1;
}

void fill_projection(GLfloat* result, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	GLfloat planeDistance = far - near;
	GLfloat inverseDistance = 1.0f / planeDistance;
	GLfloat tmp = 1.0f / tan(0.5f * fov);

	result[ 0] = tmp/aspect; result[ 1] = 		0; result[ 2] =							  0; result[ 3] =  0;
	result[ 4] = 		  0; result[ 5] = 	  tmp; result[ 6] =							  0; result[ 7] =  0;
	result[ 8] = 		  0; result[ 9] = 		0; result[10] = -(far+near)*inverseDistance; result[11] = -1;
	result[12] =		  0; result[13] =		0; result[14] =	-2*far*near*inverseDistance; result[15] =  0;
}

void fill_identity(GLfloat *result)
{
	result[ 0] = 1; result[ 4] = 0; result[ 8] = 0; result[12] =  0;
	result[ 1] = 0; result[ 5] = 1; result[ 9] = 0; result[13] =  0;
	result[ 2] = 0; result[ 6] = 0; result[10] = 1; result[14] =  0;
	result[ 3] = 0; result[ 7] = 0; result[11] = 0; result[15] =  1;
}

void fill_translate(GLfloat *result, GLfloat *vector)
{

	result[ 0] = 1; result[ 4] = 0; result[ 8] = 0; result[12] =  vector[0];
	result[ 1] = 0; result[ 5] = 1; result[ 9] = 0; result[13] =  vector[1];
	result[ 2] = 0; result[ 6] = 0; result[10] = 1; result[14] =  vector[2];
	result[ 3] = 0; result[ 7] = 0; result[11] = 0; result[15] =  vector[3];
}
