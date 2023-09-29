#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;

out vec3 color;

const float MAGNITUDE = 0.05;

void GenerateLine(int index)
{
	color = vec3(0.0);
	color[index] = 1.0;
	gl_Position = P * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = P * (gl_in[index].gl_Position + 
						vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}

const float size = 0.1;
void createVertex(vec3 offset){
	vec4 actualOffset = vec4(offset * size, 0.0);
	vec4 worldPosition = gl_in[0].gl_Position + actualOffset;
	gl_Position = P * worldPosition;
	color = vec3(0.0, 1.0, 0.0);
	EmitVertex();
}



void main()
{
/*
	for(int i=0; i<3; i++){
		gl_Position = P * gl_in[i].gl_Position;
		EmitVertex();
	}

	gl_Position = P * (gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0f));
	color = vec3(0.8, 0.2, 0.0);
	EmitVertex();


	EndPrimitive();
*/
}