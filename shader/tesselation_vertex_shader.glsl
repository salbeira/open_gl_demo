#version 450 core

layout(location = 0) in vec3 vertex;
layout(location = 0) out vec3 vsPosition;

/* Pass the vertex information to the tesselation control shader */
void main() {
	vsPosition = vertex;
}
