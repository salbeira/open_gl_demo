#version 450 core
layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 lightPos;
layout(location = 0) out vec4 out_color;
void main(){
	vec3 n = normalize(normal);
	vec3 l = lightPos;
	float df = clamp(dot(n,l), 0, 100);
	out_color = vec4(0.5 ,0.5 ,0.5 ,1.0) + df * vec4(0.01,0.01,0.01,0.0);
}
