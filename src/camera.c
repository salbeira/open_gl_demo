#include "camera.h"

#include "vector.h"

#define EYE(x) (x)
#define  AT(x) (x + 3)
#define  UP(x) (x + 6)

void move(GLfloat *camera, GLint x, GLint y, GLint z)
{
	GLfloat factor = 0.05f;	

	GLfloat zv[3] = { (AT(camera)[0] - EYE(camera)[0]),
					  (AT(camera)[1] - EYE(camera)[1]),
					  (AT(camera)[2] - EYE(camera)[2])};
	
	normalize(zv);
	GLfloat xv[3];
	cross(xv, zv, UP(camera));
	normalize(xv);
	GLfloat yv[3];
	cross(yv, zv, xv);
	normalize(yv);

	EYE(camera)[0] = EYE(camera)[0] + factor * (xv[0] * x + yv[0] * y + zv[0] * z);
	EYE(camera)[1] = EYE(camera)[1] + factor * (xv[1] * x + yv[1] * y + zv[1] * z);
	EYE(camera)[2] = EYE(camera)[2] + factor * (xv[2] * x + yv[2] * y + zv[2] * z);
}

