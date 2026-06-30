#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "errorReporting.h"
using namespace std;

#include"shaderClass.h"
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"


unsigned const int width = 1600, height = 800;

// Site-uri folositoare: 
// 1. GLFW input guide https://www.glfw.org/docs/3.3/input_guide.html 
// 2. OpenGL functions https://registry.khronos.org/OpenGL-Refpages/gl4/
// 3. OpenGL matrices https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main() {
	// --Initializari obligatorii--
	glfwInit();

	// Versiunea OpenGL 4.6 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Dezactivez mouse acceleration in GLFW
	glfwWindowHint(GLFW_STICKY_KEYS, GLFW_TRUE);
	
	GLfloat vertices[] =
	{
		// Coordonate XYZ          / Culori RGB             / TexCoord UV

		// --- BAZA PĂTRATĂ (Folosește toată textura de la 0.0 la 1.0) ---
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f, // 0: Stânga-Față
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 1.0f, // 1: Stânga-Spate
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 1.0f, // 2: Dreapta-Spate
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f, // 3: Dreapta-Față

		 // --- FAȚA 1: FAȚĂ (Triunghiul spre tine) ---
		 -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f, // 4: Stânga-Față
		  0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f, // 5: Dreapta-Față
		  0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    0.5f, 1.0f, // 6: VÂRF (Centrat sus în textură)

		  // --- FAȚA 2: SPATE ---
		  -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f, // 7: Stânga-Spate
		   0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f, // 8: Dreapta-Spate
		   0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    0.5f, 1.0f, // 9: VÂRF

		   // --- FAȚA 3: STÂNGA ---
		   -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f, // 10: Stânga-Spate
		   -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f, // 11: Stânga-Față
			0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    0.5f, 1.0f, // 12: VÂRF

			// --- FAȚA 4: DREAPTA ---
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f, // 13: Dreapta-Față
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f, // 14: Dreapta-Spate
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    0.5f, 1.0f  // 15: VÂRF
	};

	GLuint indices[] =
	{
		// Baza pătrată
		0, 1, 2,
		0, 2, 3,

		// Fețele laterale (fiecare folosește indicii ei unici)
		4, 5, 6,     // Față
		7, 8, 9,     // Spate
		10, 11, 12,  // Stânga
		13, 14, 15   // Dreapta
	};

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL 4.6", NULL, NULL); // width, height, name, screen, share
	if (window == NULL) {
		cout << "Window doesn't exist";
		glfwTerminate();	
		return -1;
	}
	// Deschide window
	glfwMakeContextCurrent(window);

	// Initializeaza OpenGL(OBLIGATORIE, daca nu atunci se va afisa eroare nullptr)
	gladLoadGL();


	// This is a customized debugger that gemini edited
	enableReportGlErrors();



	// Functie ce va retrimite informatii la OpenGL despre marimea ferestrei
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ---Shaders pentru crearea unui triunghi---
	Shader shaderProgram("default.vert", "default.frag");

	// ----Transmit informatii la GPU----
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	// Unbind
	VAO1.Unbind(); // PRIMUL TREBUIE DE SCOS VAO, apoi poti scoate si celelalte
	VBO1.Unbind();
	EBO1.Unbind();
	

	// -----Texture------	
	string texPath = "Resources/";

	Texture texture((texPath + "Image1080x1095.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);


	// Conextez uniform la textura si in acelasi timp activez shaderProgram pentru conectarea acestuia
	texture.texUnit(shaderProgram, "tex0", 0);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Prigatesc pentru a afla deltaTime pentru o miscare mai neteda
	double prevTime = glfwGetTime();

	// Pornesc testul de adancime
	glEnable(GL_DEPTH_TEST);

	

	// Loop ce tine fereastra in viata
	while (!glfwWindowShouldClose(window)) {
		// PREGATESC REDAREA OPENGL(preparing to render)
		// Curat un cadru pe fereastra, si in acelasi timp curat adancimea obiectelor
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();// Daca folosesc vre-un uniform inainte de activare, atunci nu se va primi corect

		// Calculez diferenta de timp deltaTime
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - prevTime);		
		prevTime = currentTime;

		// INTRARE
		// Primesc input de la mouse si tastatura
		camera.Inputs(window, deltaTime);
		// Apoi o transform in matrix Orientation si Position, folosind camMatrix din default.vert
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// REDARE OpenGL
		texture.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		glfwPollEvents(); // Aceasta functie ii spune la program ca sa continue sa existe(sa nu fie freeze)	
		glfwSwapBuffers(window);			
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



// Functie de a trimite informatii la OpenGL despre marimea ferestrei, DOAR dupa ce sa schimbat
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	cout << "fereastra a fost redimensionata la " << width << " pe " << height << endl;
	glViewport(0, 0, width, height);
}