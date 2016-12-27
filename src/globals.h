#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <GLFW/glfw3.h>

/* Pointers to GLFW objects */
extern GLFWwindow* window;

/* Configuration and output values */

extern unsigned int fps;
extern unsigned int active_demo;

/* Names of GL objects for geometry shader demo */
/* Not used right now
extern GLuint points_vertex_array;

extern GLuint geometry_program;
*/

/* Names of GL objects for instanced rendering demo */
/* Not used right now
extern GLuint instanced_vertex_array;

extern GLuint instanced_program;
*/
#endif
