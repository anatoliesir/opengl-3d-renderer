#include <iostream>
// INCLUDE GLAD BEFORE ANY OTHER THINGS, LIKE GLFW!!
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "errorReporting.h"



// functii:
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


using namespace std;
int main() {
	// Initializam opengl 4.6 core, glfw si glad
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "This is what hell is like", NULL, NULL);
	if (window == NULL) {
		cout << "ERROR: Failed to create a GLFWwindow*" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "ERROR: Failed to initialize GLAD" << endl;
		return -1;
	}

	// This is a customized debugger that gemini edited
	enableReportGlErrors();

	glViewport(0, 0, 800, 600);

	// It calls the function framebuffer_size_callback when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		// Check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}