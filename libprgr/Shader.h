#pragma once
#include "common.h"
#include "vectorMath.h"
#include <cstdlib>

using namespace libPRGR;

class Shader {

public:
	//Almacenar el identificador de OpenGL para este shader
	unsigned int idShader;

	//Almacena el nombre del fichero de texto con el código de este shader
	std::string fileName;

	//Tipo de shader que se ha creado: GL_VERTEX_SHADER ó GL_FRAGMENT_SHADER
	GLenum type;

	//Almacena el código del shader
	std::string source;


	//Constructor de la clase, recibe por parámetros el nombre de fichero, lo cargará y compilará llamando al resto de métodos de clase
	Shader(std::string fileName) : fileName(fileName){

		if (fileName.ends_with(".vert")) {
			type = GL_VERTEX_SHADER;
		}
		else if (fileName.ends_with(".frag")) {
			type = GL_FRAGMENT_SHADER;
		}
		else {
			std::cerr << "ERROR: Tipo de shader desconocido en " << fileName << std::endl;
		}

		readSource();
		compileShader();
		checkErrors();
	}

	// Método que abre elfichero de código de shader, lo lee y lo almacenaen la variable “source”.
	void readSource();


	//Método que compila el shader
	void compileShader();


	//Método que muestra si hay cualquier error tras compilar el shader
	void checkErrors();

	// Método que libera los datos de compilación y código del shader.Es invocado desde 
	// la clase “program” una vez se han linkado los datos del shader.
	void clean();

};