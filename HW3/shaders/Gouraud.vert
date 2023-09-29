#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPos; // Light position

uniform vec3 Ka;   // Ambient reflection coefficient
uniform vec3 Kd;   // Diffuse reflection coefficient
uniform vec3 Ks;   // Specular reflection coefficient
uniform float gloss; // Shininess: alpha
// Material color
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

out vec2 texCoord;
out vec4 ambient;
out vec4 diffuse;
out vec4 specular;

void main()
{	
	texCoord = aTexCoord;
	vec4 worldPos = V * M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(V * M));
	vec3 normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	
	gl_Position = P * V * M * vec4(aPos, 1.0);

	//hw3
	vec3 vertPos = worldPos.xyz;
	vec3 n = normalize(normal);
	vec3 l = normalize(lightPos - vertPos);
	vec3 r = reflect(-l, n);      // Reflected light vector
	vec3 v = normalize(-vertPos); // Vector to viewer

	vec4 Ka = vec4(Ka, 1.0);
	vec4 Kd = vec4(Kd, 1.0);
	vec4 Ks = vec4(Ks, 1.0);
	vec4 La = vec4(La, 1.0);
	vec4 Ld = vec4(Ld, 1.0);
	vec4 Ls = vec4(Ls, 1.0);

	ambient = La * Ka;
	diffuse = vec4(0, 0, 0, 0);
	specular = vec4(0, 0, 0, 0);
	if(dot(l, n) > 0.0) {
		diffuse = Ld * Kd * dot(l, n);
	}
	if(dot(r, v) > 0.0){
		specular = Ls * Ks * pow(dot(r, v), gloss);
	}
}