#version 450 core

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};

layout(points) in;
layout(line_strip, max_vertices = 8) out;

layout(location = 0) out vec2 texture_coordinate;

void main(){
	gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0));
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0));
	EmitVertex(); 
	EndPrimitive();

	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.0, -0.1, 0.0, 0.0));
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0));
	EmitVertex(); 
	EndPrimitive();
	
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.075, -0.075, 0.0, 0.0));
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.075, 0.075, 0.0, 0.0));
	EmitVertex(); 
	EndPrimitive();

	gl_Position = projection * (gl_in[0].gl_Position + vec4(-0.075, 0.075, 0.0, 0.0));
	EmitVertex();
	gl_Position = projection * (gl_in[0].gl_Position + vec4(0.075, -0.075, 0.0, 0.0));
	EmitVertex(); 
	EndPrimitive();
};
