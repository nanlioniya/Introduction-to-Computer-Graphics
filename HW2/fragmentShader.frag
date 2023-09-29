#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;


out vec4 FragColor;

uniform bool change_color;
uniform bool bonus;
uniform sampler2D ourTexture;

void main()
{
	vec3 lightPos = vec3(5, 10, 10);
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;

	if(change_color){
	//stripe rgb(122,56,21)
		vec4 color = texture(ourTexture, texCoord);
		if(color[0] < 0.47 && color[1]<0.21 && color[2] < 0.08) {
			//change it to black
			FragColor = vec4(0, 0, 0, 1.0);
		} else {
			FragColor = texture(ourTexture, texCoord);
		}

		if(bonus){
			float intensity;
			vec4 color;
			vec3 n = normalize(normal);
			vec3 ScreenLightPos = vec3(0.5, 0.5, 0.5);
			intensity = dot(ScreenLightPos, n);
			if(intensity>0.95) color = vec4(1.0, 0.5, 0.5, 1.0);
			else if(intensity>0.5) color = vec4(0.6, 0.3, 0.3, 1.0);
			else if(intensity>0.25) color = vec4(0.4, 0.2, 0.2, 1.0);
			else color = vec4(0.2, 0.1, 0.1, 1.0);
			FragColor = color;
		}
	}
	else if(bonus){
		float intensity;
		vec4 color;
		vec3 n = normalize(normal);
		vec3 ScreenLightPos = vec3(0.5, 0.5, 0.5);
		intensity = dot(ScreenLightPos, n);
		if(intensity>0.95) color = vec4(1.0, 0.5, 0.5, 1.0);
		else if(intensity>0.5) color = vec4(0.6, 0.3, 0.3, 1.0);
		else if(intensity>0.25) color = vec4(0.4, 0.2, 0.2, 1.0);
		else color = vec4(0.2, 0.1, 0.1, 1.0);
		FragColor = color;
	}
	else{
		FragColor = texture(ourTexture, texCoord);
	}
}