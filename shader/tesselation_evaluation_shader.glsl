#version 450 core
layout(std140) uniform Matrices {
	mat4 projection;
	mat4 camera;
	mat4 model;
};
layout(triangles, equal_spacing, cw) in;
layout(location = 0) in vec3 tcsPosition[];
layout(location = 0) out vec3 tesPosition;
void main(){
	vec3 p0 = gl_TessCoord.x * tcsPosition[0];
	vec3 p1 = gl_TessCoord.y * tcsPosition[1];
	vec3 p2 = gl_TessCoord.z * tcsPosition[2];
	vec3 pos = normalize (p0 + p1 + p2);
	tesPosition = pos;
	gl_Position = projection * camera * model * vec4(pos, 1.0);
}
