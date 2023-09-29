#version 330 core

/*
in vec2 texCoord;
in vec3 normal;
in vec3 vertPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos; // Light position

uniform vec3 Ka;   // Ambient reflection coefficient
uniform vec3 Kd;   // Diffuse reflection coefficient
uniform vec3 Ks;   // Specular reflection coefficient
uniform float gloss; // Shininess: alpha
// Material color
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

out vec4 FragColor;
*/

in vec2 texCoord;
in vec3 normal;
in vec3 vertPos;

uniform sampler2D ourTexture;

uniform vec3 Ka;   // Ambient reflection coefficient
uniform vec3 Kd;   // Diffuse reflection coefficient
uniform vec3 Ks;   // Specular reflection coefficient
uniform float gloss; // Shininess: alpha
// Material color
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform vec3 lightPos; // Light position

out vec4 FragColor;


void main()
{
	/*
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

	vec4 color = texture(ourTexture, texCoord);
	vec4 ambient = La * Ka * color;
	float lambertian = max(dot(n, l), 0.0);
	vec4 diffuse = vec4(0, 0, 0, 0);
	vec4 specular = vec4(0, 0, 0, 0);
	if(lambertian > 0.0) {
		// Compute the specular term
		diffuse = Ld * Kd * color * dot(l, n);
		//float specAngle = max(dot(r, v), 0.0);
		//specular = Ls * Ks * pow(specAngle, gloss);
		specular = Ls * Ks * pow(dot(r, v), gloss);
	}
	FragColor = ambient + diffuse + specular;
	*/
	
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

	vec4 color = texture(ourTexture, texCoord);
	vec4 ambient = La * Ka * color;
	vec4 diffuse = vec4(0, 0, 0, 0);
	vec4 specular = vec4(0, 0, 0, 0);

	if(dot(l, n) > 0.0) {
		diffuse = Ld * Kd * color * dot(l, n);
	}
	if(dot(r, v) > 0.0){
		specular = Ls * Ks * pow(dot(r, v), gloss);
	}

	FragColor = ambient + diffuse + specular;

}