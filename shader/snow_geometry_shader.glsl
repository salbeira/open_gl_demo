#version 450 core

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) out vec2 texture_coordinate;

void main(){
	gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0));
	texture_coordinate = vec2(0.0, 0.0);
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.2, -0.2, 0.0, 0.0));
	texture_coordinate = vec2(1.0, 0.0);
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.2, 0.2, 0.0, 0.0));
	texture_coordinate = vec2(0.0, 1.0);
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.2, 0.2, 0.0, 0.0));
	texture_coordinate = vec2(1.0, 1.0);
	EmitVertex();
	EndPrimitive();
};
