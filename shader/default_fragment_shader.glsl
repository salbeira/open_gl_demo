#version 450 core

uniform sampler2D texture_color;

layout(location = 0) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 out_color;

void main(){
	out_color = texture2D(texture_color, fs_texture_coordinate);
} 
