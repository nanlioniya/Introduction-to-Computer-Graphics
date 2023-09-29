#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

out vec3 normal;
out vec2 texCoord;
uniform mat4 P;


float random(float x)
{
    float y = fract(sin(x)*100000.0);
    return y;
}


uniform float time;
vec4 explode(vec4 position, vec3 normal){
	vec3 direction;
	
	if(time < 25.2){
		direction = normal * sin(time / 2.0)*2;

	}
	else if (time >=25.2 && time < 37){
		direction = normal * sin(time / 2.0)*1.5;
		if(position[0]> 0) direction[0] = direction[0]*-1;
		else if(position[0]<0) direction[1] = direction[1]*-1;
		else direction[2] = direction[2]*-1;
	}
	else if (time >= 37 && time<56.5){
		direction = normal * sin(time / 2.0)*1.5;
		if(position[0]< 0) direction[0] = direction[0]*-1.5;
		else if(position[1]<0) direction[0] = direction[1]*-1.5;
		else if(position[2]<0) direction[0] = direction[2]*1.5;	
		else if(position[0]>0) direction[0] = direction[0]*-0.5;
		else if(position[1]>0) direction[1] = direction[1]*1.5;
		else if(position[2]>0) direction[1] = direction[2]*-1.6;	
		else if(position[0]<0) direction[1] = direction[0]*-0.1;
		else if(position[1]<0) direction[1] = direction[1]*-1.3;
		else if(position[2]<0) direction[2] = direction[2]*1.8;	
		else if(position[0]>0) direction[2] = direction[0]*0.7;
		else if(position[1]>0) direction[2] = direction[1]*-0.3;
		else direction[2] = direction[2]*-0.2;

	}


	return position + vec4(direction, 0.0);
} 


void main()
{
	
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 normal = normalize(cross(b, a));

	/*
	for (int i = 0; i < 3; i++)
	{
		gl_Position = P * explode(gl_in[i].gl_Position, normal);
		normal = gs_in[i].normal;
		texCoord = gs_in[i].texCoord;
		EmitVertex();
	}
	*/


	gl_Position = P * explode(gl_in[1].gl_Position, normal);
	vec4 origin = gl_Position;
	normal = gs_in[1].normal;
	texCoord = gs_in[1].texCoord;
	EmitVertex();

	gl_Position = origin + vec4(a, 0.0);
	normal = gs_in[0].normal;
	texCoord = gs_in[0].texCoord;
	EmitVertex();

	gl_Position = origin + vec4(b, 0.0);
	normal = gs_in[2].normal;
	texCoord = gs_in[2].texCoord;
	EmitVertex();

	gl_Position = origin + vec4(0.1, 0.0, 0.0, 0.0f);
	normal = gs_in[2].normal;
	texCoord = gs_in[2].texCoord;
	EmitVertex();


	EndPrimitive();
}