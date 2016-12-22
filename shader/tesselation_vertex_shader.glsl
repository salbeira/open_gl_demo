#version 450 core

layout(location = 0) in vec3 vertex;
layout(location = 0) out vec3 vsPosition;

void main() {
	vsPosition = vertex;
}
