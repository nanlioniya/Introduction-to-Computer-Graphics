#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;


void main()
{	
	texCoord = aTexCoord;
	worldPos = V * M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(V * M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	
	gl_Position = P * V * M * vec4(aPos, 1.0);
}