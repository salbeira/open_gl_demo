#include "load.h"

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

char *read_file(const char *filename)
{
	char *buffer = NULL;
	size_t size = 0;

	FILE *file = fopen(filename, "r");

	if(!file)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fopen");
		return NULL;
	}
	
	if(fseek(file, 0, SEEK_END))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fseek");
		return NULL;
	}
	size = ftell(file);
	if(size == -1)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] ftell");
		return NULL;
	}
	rewind(file);

	buffer = malloc(sizeof(char) * (size+1));
	if(!buffer)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] malloc");
		return NULL;
	}

	if(!fread(buffer, size, 1, file))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fread");
		return NULL;
	}

	buffer[size] = '\0';

	if(fclose(file))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fclose");
		return NULL;
	}

	return buffer;
}

void read_file_data(size_t *length, char **buffer, const char *filename)
{
	FILE *file = fopen(filename, "r");
	size_t local_size;
	char *local_buffer;
	if(!file)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fopen");
		return;
	}
	
	if(fseek(file, 0, SEEK_END))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fseek");
		return;
	}

	local_size = ftell(file);
	if(local_size == -1)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] ftell");
		return;
	}

	rewind(file);

	local_buffer = malloc(sizeof(char) * (local_size+1));
	if(!buffer)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] malloc");
		return;
	}

	if(!fread(local_buffer,local_size, 1, file))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fread");
		return;
	}

	local_buffer[local_size] = '\0';

	if(fclose(file))
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		perror("[ERROR] fclose");
		return;
	}
	*length = local_size;
	*buffer = local_buffer;
}

GLuint compile_shader(const char *filename, GLuint type)
{
	char *source = NULL;
	size_t length = 0;
	GLint success;

	read_file_data(&length, &source, filename);
	if(source == NULL || length == 0)
	{
		fprintf(stderr, "[ERROR] %s\n", filename);
		fprintf(stderr, "[ERROR] read_file_data\n");
		return 0;
	}
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1,(const char const * const*) &source,(const GLint const*) &length);
	glCompileShader(shader);
	free(source);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = (GLchar *) malloc(log_length * sizeof(GLchar));
		glGetShaderInfoLog(shader, log_length, &log_length, log);
		printf("[ERROR] glCompileShader %s:\n%s\n", filename, log);
		free(log);
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;	
}

void read_png_data(GLvoid **data, GLuint *width, GLuint *height, const char *filename)
{
	png_byte header[8];

	FILE *fp = fopen(filename, "rb");
	    if (fp == 0)
    {
        perror(filename);
        return;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", filename);
        fclose(fp);
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*) malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_bytep*) malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    unsigned int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
	
	*width = temp_width;
	*height = temp_height;
	*data = (GLvoid*) image_data;
}
