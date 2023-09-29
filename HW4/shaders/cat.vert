#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 normalM;

out VS_OUT {
	vec3 normal;
	vec2 texCoord;
} vs_out;



void main()
{
	gl_Position = V * M * vec4(aPos, 1.0);
	vs_out.texCoord = aTexCoord;
	vs_out.normal = normalize((normalM * vec4(aNormal, 0.0)).xyz);;

}