#pragma once
#pragma once
#include "common.h"

class Texture {

public:

	typedef struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	}pixel_t;

	string fileName = "";
	std::vector<pixel_t> pixels;
	int w = 0;
	int h = 0;

	//datos openGL
	unsigned int textureID = -1;

	Texture(string fileName) : fileName(fileName) {
		//cargar datos de fichero.
		loadFile(fileName);
		//copiar datos a GPU.
		updateGPU();

	}

	void loadFile(string fileName);
	void updateGPU();

};