#include"Camera.h"
#include<string>
#include<iostream>

// Constructia default
Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Creez matricea view(Unde se uita camera)
	view = glm::lookAt(Position, // Pozitia camerei
		Position + Orientation, // Te uiti in directia Orientation
		Up); // Care este vectorul ce arata sus

	// Creez matricea proiectie(Perspectiva 3D)
	projection = glm::perspective(glm::radians(FOVdeg), // FOV in radieni(Ex: 45 grade)
		(float)width / height, // Aspect ratio
		nearPlane, // Cea mai apropiata distanta de la care pot vedea (Ex: 0.1f)
		farPlane); // Cea mai departe distanta de la care pot vedea (Ex: 100.0f)

	// Trimite matricea combinata la shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime) {	
	// Calculez viteza curenta pentru un fps constant.
	float currentSpeed = speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += currentSpeed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += currentSpeed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += currentSpeed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += currentSpeed * glm::normalize(glm::cross(Orientation, Up));
	}

	// SUS (AMOGUS)
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += currentSpeed * Up;
	}
	// Jos
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += currentSpeed * -Up;
	}

	// Schimb viteza prin shift
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		currentSpeed = speed * deltaTime + 0.1;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		currentSpeed = speed * deltaTime;
	}

	
	// Mouse control(cand APESI butonul stang)	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{		
		double mouseX;
		double mouseY;		
		if (firstClick)
		{
			// Ascunde cursorul
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Obtin pozitia mouse-ului
			glfwGetCursorPos(window, &mouseX, &mouseY);
			firstClick = false;
			mouseXTemp = mouseX;
			mouseYTemp = mouseY;
		}		
		else {
			// Obtin pozitia mouse-ului
			glfwGetCursorPos(window, &mouseX, &mouseY);
		}

		// Calculez rotatia
		//float verticalLook = sensitivity *(float)(mouseY - (height / 2)) / height;// in jurul axei Y
		//float horizontalLook = sensitivity * (float)(mouseX - (width / 2)) / width;// in jurul axey X
		float verticalLook = sensitivity * (float)(mouseY - mouseYTemp);// in jurul axei Y
		float horizontalLook = sensitivity * (float)(mouseX - mouseXTemp);// in jurul axey X


		// Rotire verticala
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-verticalLook), glm::normalize(glm::cross(Orientation, Up)));

		// Limitezi rotatia verticala, sa nu fie mai mult de 85.0f grade prea jos/sus
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-horizontalLook), Up);

		mouseXTemp = mouseX;
		mouseYTemp = mouseY;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
		
}
