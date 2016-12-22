#version 450 core
layout(std140) uniform Tesselation {
	int inner;
	int outer;
};
layout(vertices = 3) out;
layout(location = 0) in vec3 vsPosition[];
layout(location = 0) out vec3 tcsPosition[];

#define ID gl_InvocationID

void main(){
	tcsPosition[ID] = vsPosition[ID];
	if(ID == 0) {
		gl_TessLevelInner[0] = inner;
		gl_TessLevelOuter[0] = outer;
		gl_TessLevelOuter[1] = outer;
		gl_TessLevelOuter[2] = outer;
	}
}
