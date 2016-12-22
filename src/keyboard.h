#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <GLFW/glfw3.h>

#include "globals.h"
/* Keyboard buttons and values changed by it */

extern unsigned char key_up;
extern unsigned char key_down;
extern unsigned char key_left;
extern unsigned char key_right;
extern unsigned char key_high;
extern unsigned char key_low;

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods);

#endif
