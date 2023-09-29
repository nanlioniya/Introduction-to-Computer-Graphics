#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void loadMaterialLight();
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
void loadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* catModel = new Object("obj/cat_l25.obj");

Material material;
Light light;
glm::vec3 cameraPos = glm::vec3(0, 3.5, 3.5);

int windowWidth = 800, windowHeight = 600;
int phongProgram, toonProgram, edgeProgram, gouraudProgram, currentProgram;

unsigned int ModelVAO(Object* model);
void DrawModel(glm::mat4 M, glm::mat4 V, glm::mat4 P);
unsigned int catVAO;
unsigned int shaderProgram;
unsigned int catTexture;


// 1: phong, 2:gouraud, 3:toon, 4:edge
int key_input = 1;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// TODO:
	// Create shaders
	unsigned int vertexShader, fragmentShader;
	


	// Texture
	loadTexture(catTexture, "obj/Cat_diffuse.jpg");

	// VAO, VBO
	catVAO = ModelVAO(catModel);

	// Display loop
	loadMaterialLight();
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glm::mat4 view = getView();
	glm::mat4 perspective = getPerspective();

	

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.4f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (key_input == 1) {
			vertexShader = createShader("shaders/Phong.vert", "vert");
			fragmentShader = createShader("shaders/Phong.frag", "frag");
		}
		else if (key_input == 2) {
			vertexShader = createShader("shaders/Gouraud.vert", "vert");
			fragmentShader = createShader("shaders/Gouraud.frag", "frag");
		}
		else if (key_input == 3) {
			vertexShader = createShader("shaders/Toon.vert", "vert");
			fragmentShader = createShader("shaders/Toon.frag", "frag");
		}
		else{
			vertexShader = createShader("shaders/Edge.vert", "vert");
			fragmentShader = createShader("shaders/Edge.frag", "frag");
		}
		shaderProgram = createProgram(vertexShader, fragmentShader);
		glUseProgram(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

		// TODO:
		// Draw the cat with current active shader
		glm::mat4 cat_model = glm::mat4(1.0f);//model mat
		glm::mat4 cat_model_R = glm::mat4(1.0f);//for rotation

		cat_model_R = glm::rotate(cat_model_R, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0, 1, 0));//Rotate about +y axis -0.2 degree/frame
		cat_model = cat_model_R;

		DrawModel(cat_model, view, perspective);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void DrawModel(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");

	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));
	
	if (key_input == 1) {
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ka"), 1, glm::value_ptr(material.ambient));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Kd"), 1, glm::value_ptr(material.diffuse));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ks"), 1, glm::value_ptr(material.specular));
		glUniform1f(glGetUniformLocation(shaderProgram, "gloss"), material.gloss);
		glUniform3fv(glGetUniformLocation(shaderProgram, "La"), 1, glm::value_ptr(light.ambient));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ld"), 1, glm::value_ptr(light.diffuse));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ls"), 1, glm::value_ptr(light.specular));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
	}
	else if (key_input == 2) {
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ka"), 1, glm::value_ptr(material.ambient));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Kd"), 1, glm::value_ptr(material.diffuse));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ks"), 1, glm::value_ptr(material.specular));
		glUniform1f(glGetUniformLocation(shaderProgram, "gloss"), material.gloss);
		glUniform3fv(glGetUniformLocation(shaderProgram, "La"), 1, glm::value_ptr(light.ambient));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ld"), 1, glm::value_ptr(light.diffuse));
		glUniform3fv(glGetUniformLocation(shaderProgram, "Ls"), 1, glm::value_ptr(light.specular));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
	}
	else if (key_input == 3) {
		glUniform3fv(glGetUniformLocation(shaderProgram, "Kd"), 1, glm::value_ptr(material.diffuse));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
	}
	else {

	}



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, catTexture);
	glBindVertexArray(catVAO);
	glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	
	glBindVertexArray(0);
}

void loadMaterialLight() {
	material.ambient = glm::vec3(1.0, 1.0, 1.0);
	material.diffuse = glm::vec3(1.0, 1.0, 1.0);
	material.specular = glm::vec3(0.7, 0.7, 0.7);
	material.gloss = 10.5;

	light.ambient = glm::vec3(0.2, 0.2, 0.2);
	light.diffuse = glm::vec3(0.8, 0.8, 0.8);
	light.specular = glm::vec3(0.5, 0.5, 0.5);
	light.position = glm::vec3(10, 10, 10);
}


// TODO:
// Add key callback to switch between shaders
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		key_input = 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		key_input = 2;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		key_input = 3;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		key_input = 4;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << filename << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}


void loadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		cameraPos,
		glm::vec3(0, 0.5, 0),
		glm::vec3(0, 1, 0));
}

unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}