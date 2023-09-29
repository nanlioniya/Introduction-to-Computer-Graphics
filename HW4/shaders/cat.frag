#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;
uniform float time;

out vec4 FragColor;

void main()
{

	if(time < 25.2){
		FragColor = texture(ourTexture, texCoord);
	}
	else if (time >=25.2 && time < 37){
		FragColor = texture(ourTexture, texCoord);
		if (time >=25.2 && time < 28){
		/*
			if(FragColor[0]>0.1)  FragColor[0] = 0.1;
			else if(FragColor[1]>0.1)  FragColor[1] = 0.4;
			else if(FragColor[2]>0.1)  FragColor[2] = 0.9;
			else FragColor[0] = 0.2;*/
		}
		else{
		/*
			if(FragColor[0]>0.5)  FragColor[0] = abs(sin(time));
			if(FragColor[1]>0.1)  FragColor[1] =  abs(cos(time*4));
			if(FragColor[2]>0.4)  FragColor[2] =  abs(cos(time*2));*/
		}
	}
	else if (time >= 37 && time<56.5){
		FragColor = texture(ourTexture, texCoord);
		if (time >= 37 && time < 48){
			if(FragColor[0]>0.3)  FragColor[0] = abs(tan(time*6));
			else if(FragColor[1]>0.3)  FragColor[1] =  abs(cos(time*2));
			else FragColor[2] = abs(cos(time*2));
		}
		else if (time>=48){
			/*
			FragColor[0] = abs(sin(time*5));
			FragColor[1] =  abs(cos(time*2));
			FragColor[2] =  abs(sin(time*2));*/
			
			/*
			FragColor[0] = abs(sin(time*3))+0.1;
			FragColor[1] =  abs(cos(time*2))+0.1;
			FragColor[2] =  abs(sin(time*6))+0.1;*/
			
			if(FragColor[0]>0.5)  FragColor[0] = abs(sin(time));
			if(FragColor[1]>0.1)  FragColor[1] =  abs(cos(time*4));
			if(FragColor[2]>0.4)  FragColor[2] =  abs(cos(time*2));

		}
	}
}