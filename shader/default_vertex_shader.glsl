#version 450 core

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec2 in_texture_coordinate;

layout(location = 0) out vec2 fs_texture_coordinate;

void main(){
	fs_texture_coordinate = in_texture_coordinate;
	gl_Position = projection * camera * model * vec4(in_vertex, 1.0);
}
