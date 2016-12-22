#version 450 core

layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
layout(location = 0) in vec3 tesPosition[3];
layout(location = 0) out vec3 normal;
layout(location = 1) out vec3 lightPos;

void main(){
	vec3 a = tesPosition[2] - tesPosition[0];
	vec3 b = tesPosition[1] - tesPosition[0];
	mat4 mat_cm = camera * model;
	mat4 normal_mat = transpose(inverse(mat_cm));
	vec3 c = normalize(cross(a,b));
	vec4 d = vec4(c,1.0);
	normal = (normal_mat * d).xyz;
	lightPos = (camera * vec4(10.0, 10.0, 10.0, 1.0)).xyz;
	gl_Position = gl_in[0].gl_Position; EmitVertex();
	gl_Position = gl_in[1].gl_Position; EmitVertex();
	gl_Position = gl_in[2].gl_Position; EmitVertex();
	EndPrimitive();
};
