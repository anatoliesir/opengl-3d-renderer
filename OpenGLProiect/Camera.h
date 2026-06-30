#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<string>

#include"shaderClass.h"

class Camera {
public:

	glm::vec3 Position;// Pozitia camerei in lume
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);// Unde se uita camera
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);// Care este vectorul de "sus". La noi este y
	
	int width, height;// Dimensiunile ferestrelor
	bool firstClick = true;// Evita "sarituri" la primul click

	float speed = 3.0f, sensitivity = 0.015f;// viteza camerei si sensibilitatea(sensibilitatea este grad/pixel)

	// Variabila temporara pentru aflarea pozitiei cursorului din trecut
	double mouseXTemp, mouseYTemp;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window, float deltaTime);	
	void CameraDebugging(GLFWwindow* window);
};


#endif
