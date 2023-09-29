#version 330 core
/*
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 texCoord;
out vec3 normal;
out vec3 vertPos;
*/
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 texCoord;
out vec3 normal;
out vec3 vertPos;


void main()
{	
	texCoord = aTexCoord;
	vec4 worldPos = V * M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(V * M));
	normal = (normal_transform * vec4(aNormal, 0.0)).xyz;
	vertPos = worldPos.xyz;

	gl_Position = P * V * M * vec4(aPos, 1.0);

}