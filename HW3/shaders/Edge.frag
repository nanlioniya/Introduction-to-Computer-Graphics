#version 330 core

in vec3 normal;
in vec3 vertPos;

out vec4 FragColor;

void main()
{
	vec3 n = normalize(normal);
	vec3 v = normalize(vertPos); // Vector from viewer

	vec4 color;
	if(dot(n, v) >= -0.2) color = vec4(0.25, 0.53, 0.95, 1.0);
	else color = vec4(0.0,0.0,0.0,1.0);

	FragColor = color;
}