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

// provided by TA
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
void LoadTexture(unsigned int& texture, const char* tFileName);
Object* catModel = new Object("obj/cat.obj");
Object* boxModel = new Object("obj/CardboardBox1.obj");
int windowWidth = 800, windowHeight = 600;

//added
unsigned int ModelVAO(Object* model);
void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P);

glm::mat4 getPerspective();
glm::mat4 getView();

unsigned int catVAO, boxVAO;
unsigned int shaderProgram;
unsigned int catTexture, boxTexture;

// effets and bonus
bool squeeze_cat = false;
bool change_color = false;
bool bonus = false;

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

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2", NULL, NULL);
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

	// Shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// Texture
	LoadTexture(catTexture, "obj/Cat_diffuse.jpg");
	LoadTexture(boxTexture, "obj/CardboardBox1_Albedo.tga");


	// TO DO:
	// Create VAO, VBO
	catVAO = ModelVAO(catModel);
	boxVAO = ModelVAO(boxModel);

	// Display loop
	// TO DO:
	// Draw the models with shaders
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glm::mat4 view = getView();
	glm::mat4 perspective = getPerspective();

	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*******box********/
		glm::mat4 box_model = glm::mat4(1.0f);//model mat
		glm::mat4 box_model_S = glm::mat4(1.0f);//for scale
		glm::mat4 box_model_R = glm::mat4(1.0f);//for rotation

		box_model_S = glm::scale(box_model_S, glm::vec3(0.0625, 0.05, 0.05));//scale(0.0625, 0.05, 0.05)
		box_model_R = glm::rotate(box_model_R, (float)glfwGetTime()*glm::radians(90.0f), glm::vec3(0, 1, 0));//Rotate about +y axis -0.2 degree/frame

		box_model = box_model_R*box_model_S;

		/*******cat********/
		glm::mat4 cat_model = glm::mat4(1.0f);//model mat
		glm::mat4 cat_model_R = glm::mat4(1.0f);//for rotation

		cat_model_R = glm::rotate(cat_model_R, glm::radians(90.0f), glm::vec3(0, 1, 0));//Rotate about +y axis -0.5 degree/frame
		cat_model = box_model_R * cat_model_R;

		DrawModel("cat", cat_model, view, perspective);
		DrawModel("box", box_model, view, perspective);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	unsigned int mLoc, vLoc, pLoc, sLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");

	//for scale
	glm::mat4 cat_model_S = glm::mat4(1.0f);
	glm::mat4 cat_model_T = glm::mat4(1.0f);
	cat_model_T = glm::translate(cat_model_T, glm::vec3(0, 0.1, 0));//scale(0.0625, 0.05, 0.05)

	if (squeeze_cat) {
		M = M * cat_model_T;
		cat_model_S = glm::scale(cat_model_S, glm::vec3(1, 1 + (sin((float)glfwGetTime()))/2, 1));//scale(0.0625, 0.05, 0.05)
	}
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));

	sLoc = glGetUniformLocation(shaderProgram, "S");


	if (bonus) {
		glUniform1i(glGetUniformLocation(shaderProgram, "bonus"), true);
		glUniform1f(glGetUniformLocation(shaderProgram, "time"), glfwGetTime());
		glUniform1f(glGetUniformLocation(shaderProgram, "xs"), 8.0);
		glUniform1f(glGetUniformLocation(shaderProgram, "zs"), 10.0);
	}
	else {
		glUniform1i(glGetUniformLocation(shaderProgram, "bonus"), false);
	}

	if (strcmp(target, "box") == 0)
	{

		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniform1i(glGetUniformLocation(shaderProgram, "test"), false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		glBindVertexArray(boxVAO);
		glDrawArrays(GL_TRIANGLES, 0, boxModel->positions.size());
	}
	else if (strcmp(target, "cat") == 0)
	{

		if (change_color) {
			//float greenValue = (sin((float)glfwGetTime()) / 2.0f) + 0.5;
			glUniform1i(glGetUniformLocation(shaderProgram, "change_color"), true);
		}
		else {
			glUniform1i(glGetUniformLocation(shaderProgram, "change_color"), false);
		}


		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(cat_model_S));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, catTexture);
		glBindVertexArray(catVAO);
		glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	}

	glBindVertexArray(0);
}	


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (squeeze_cat) {
			squeeze_cat = false;
			cout << "Press 1: effect1 close\n";
		}
		else {
			squeeze_cat = true;
			cout << "Press 1: effect1 open\n";
		}
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (change_color) {
			change_color = false;
			cout << "Press 2: effect2 close\n";
		}
		else {
			change_color = true;
			cout << "Press 2: effect2 open\n";
		}
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (bonus) {
			bonus = false;
			cout << "Press 3: bonus close\n";
		}
		else {
			bonus = true;
			cout << "Press 3: bonus open\n";
		}
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
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
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

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);//產生獨一無二的ID
	glBindTexture(GL_TEXTURE_2D, texture);//然後綁定
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
		0.1f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		glm::vec3(0, 5, 5),
		glm::vec3(0, 0, 0),
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