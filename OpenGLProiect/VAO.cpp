#include"VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}


// layout - location ce este folosit in shaderProgram in fisierul default.vert

// numComponents - cate componente sunt transmise la GPU

// type - tipul de variabila(in cazul nostru este GL_FLOAT)

// stride - marimea in octeti, anume DIMENSIUNEA COMPLETA AL UNUI VERTEX

// offset - (void*)0 inseamna ca incepe de la pointer cu valoarea 0,

// dar poate fi schimbat pentru ca sa incepe in alta parte, de exemplu (void*)(3 * sizeof(float))
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
void VAO::Bind() {
	glBindVertexArray(ID);
}
void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}
