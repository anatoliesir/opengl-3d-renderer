#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum pixelType)
{
	// texType este de obicei GL_TEXTURE_2D
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);

	// Citesc imaginea si il stochez in biti
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generez textura
	glGenTextures(1, &ID);

	// Cand activez, il pun intrun slot anumit la textura(Ex: GL_TEXTURE0, GL_TEXTURE1)
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configurez tipul de algoritm pentru a face imaginea mai mare sau mai mica
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configurez modul in care textura se repeta pe sine
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Formatul imaginii este aflat prin numarul de canale de culori are imaginea(numColCh)
	GLenum format = GL_RGB;
	if (numColCh == 1)
		format = GL_RED;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 4)
		format = GL_RGBA;
	// PixelTupe este tipul de valoare(Ex: GL_UNSIGNED_BYTE)
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);

	// Generez versiuni mai mici si mai mari al imaginii
	glGenerateMipmap(texType);

	// sterg bitii, din cauza ca aceasta informatie este deja in OpenGL
	stbi_image_free(bytes);

	// Unbind textura
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader-ul trebuie activat inainte de schimbarea uniformului
	shader.Activate();	
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}