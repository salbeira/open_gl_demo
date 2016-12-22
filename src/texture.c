#include "texture.h"
#include <stdio.h>

GLuint create_texture(GLuint width, GLuint height, GLvoid* data)
{
	GLint filter = GL_LINEAR;
	GLint wrap = GL_REPEAT;

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	printf("[INFO] Created a new texture with dimensions (%d, %d)\n", width, height);

	return texture_id;
}
