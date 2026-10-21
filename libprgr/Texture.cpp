#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/stb_image.h>

void Texture::loadFile(string fileName)
{
	int channel = 4;
	//Cargar datos de fichero.
	unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &channel, 4);
	// si no hay error 
	if (data != nullptr)
	{
		//copiar a vector de datos de pixel
		pixels.resize(w * h); //redimensionar a ancho(w) por alto(h)
		memcpy(pixels.data(), data, pixels.size() * sizeof(pixel_t)); //copiar datos en fichero.
		stbi_image_free(data); // borra datos de imagen.

	}
	else {
		cout << "ERROR: " << __FILE__ << ":" << __LINE__ << "fichero no encontrado" << endl;
	}

}

void Texture::updateGPU()
{

	//crear identificador de textura
	if (textureID == -1) {
		glGenTextures(1, &textureID);

	}

	//bindear/atar textura a unidad activa
	glBindTexture(GL_TEXTURE_2D, textureID);
	//set propiedades de filtro de imagen
	//GL_NEAREST nos quita la borrosidad debido a que son pixeles, el GL_LINEAR lo que hace es hacer 
	// difuminado de pixeles.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Filtro bilinear por defecto
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//set propiedades de repeticion de imagen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeticion eje X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// Repeticion eje Y
	//copiar a GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}
