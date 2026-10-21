#pragma once
#include "vectorMath.h"
#include "texture.h"

using namespace libPRGR;
using namespace std;
class Material {

public:


	//Nombre del fichero  de textura  que se usará  para colorear el objeto
	string textureName;

	Texture* texture;
	//Identificador de la textura que se usará para colorear el objeto.
	unsigned int textureId;

	//Tamaños en píxeles de la textura cargada
	int width, height;
	//Pulido del material. 
	int shinny = 20;
	//Constantes de iluminación del material
	float ka, kd, ks;

	// Constructor de la clase, a partir de un nombre de fichero de textura y sus constantes
	// de iluminación.Una vez invocado, cargará los datos de textura del fichero que
	// recibe por parámetros, usando los métodos de opengl vistos en clase.
	Material(std::string textureName) : textureName(textureName), ka(ka), kd(kd), ks(ks) {

		texture = new Texture(textureName);

	}

	// Método que inicializa una unidad de texturado usando como índice el número que
	// se le pasa por parámetros
	void bind(int textureUnit);

};