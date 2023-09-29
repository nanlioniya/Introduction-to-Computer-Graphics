#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 S;

//bonus
uniform float time;
uniform float xs, zs;
uniform bool bonus;


out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;


void main()
{	
	texCoord = aTexCoord;
	worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	
	vec3 object_pos;
	float s;
	s = 1.0 + 0.1*sin(xs*time)*sin(zs*time);
	object_pos.x = s*aPos.x;
	object_pos.y = s*aPos.y;
	object_pos.z = s*aPos.z;

	if(bonus){
		gl_Position = P * V * M * S * vec4(object_pos, 1.0);
	}
	else{
		gl_Position = P * V * M * S * vec4(aPos, 1.0);
	}
}