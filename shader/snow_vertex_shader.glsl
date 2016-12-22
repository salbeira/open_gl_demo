#version 450 core

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};

layout(location = 0) in vec3 in_vertex;

void main(){
	gl_Position = camera * model * vec4(in_vertex, 1.0);
}
