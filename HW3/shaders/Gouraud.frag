#version 330 core

in vec2 texCoord;
in vec4 ambient;
in vec4 diffuse;
in vec4 specular;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	vec4 color = texture(ourTexture, texCoord);

	vec4 frag_ambient = color * ambient;
	vec4 frag_diffuse = color * diffuse;
	vec4 frag_specular = specular;

	FragColor = frag_ambient + frag_diffuse + frag_specular;
}