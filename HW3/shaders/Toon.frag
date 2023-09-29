#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform vec3 Kd;
uniform vec3 lightPos;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	float intensity;
	vec4 color;
	vec4 object_color = texture(ourTexture, texCoord);
	vec3 n = normalize(normal);
	vec3 vertPos = vec3(worldPos) / worldPos.w;
	vec3 l = normalize(lightPos - vertPos);
	intensity = dot(l, n);

	vec4 Kd = vec4(Kd, 1.0);

	
	if(intensity>=0.7) color = vec4(1.0, 1.0, 1.0, 1.0);
	else if(intensity>=0.3) color = vec4(0.6, 0.6, 0.6, 1.0);
	else color = vec4(0.2, 0.2, 0.2, 1.0);
	FragColor = Kd * object_color * color;
}