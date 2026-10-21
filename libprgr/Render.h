#pragma once
#include "common.h"
#include "vectorMath.h"
#include <cstdlib>
#include "Object3D.h"
#include "EventManager.h"
#include "Camera.h"
#include "OrbitalLight.h"
#include "light.h"
#include <chrono>
#include <thread>

using namespace libPRGR;


/*
#ifdef _WIN32
#define SO "Windows"
#include <windows.h>
#define DORMIR Sleep(2000)
#elif __linux__ || __APPLE__
#define SO "Linux/Apple"
#include <unistd.h>
#define DORMIR sleep(2)
#endif

#define MODIFICAR if (i== centroX) {buffer[i][j] = '-';\
}else if (j == centroY) {buffer[i][j] = '|';}\
else {buffer[i][j] = '0';}
*/


class Render {
public:
	typedef struct {
		unsigned int idArray; //Identificador de grupos de Array 
		unsigned int idVertexArray; // Identificador de datos de vertice
		unsigned int idIndexArray; // Identificador de orden de vertices
	}bufferObject;


	int alto, ancho;
	int centroX, centroY;
	char** buffer;
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	Light* light;
	std::vector<Light*> lights;
	std::map<Object3D*, Light*> lucesAsociadas;
	std::vector<Object3D*> objectList;
	std::map<int, bufferObject> bufferObjects;

	Render(int alto, int ancho);
	// Activará un pixel(pondrá el valor 1) en la posición "x,y" de la matriz
	void PutPixel(int x, int y);
	// Limpiará el buffer actual(pondrá un 0 en todas las posiciones de la matriz)
	void ResetBufer();


	GLFWwindow* initGL(int horizontal, int vertical);

	void putLight(Light* light);

	void putCamera(Camera* camj);
	void setupObject(Object3D* obj);
	//
	//Método que añade un objeto a la lista de objetos.Debe crear su bufferObject y
	//almacenar en el mapa bufferObjets los identificadores de buffers creados / cargados end
	//GPU
	//
	void putObject(Object3D* obj);

	//
	//Método que elimina un objeto a la lista de objetos.Debe destruir su bufferObject y
	//buffers creados / cargados en GPU.

	//
	void removeObject(Object3D* obj);
	//
	//	Método que dibuja los objetos en la ventana de openGL.Este método recorre la lista de
	//	objetos, y por cada uno de ellos los carga en GPU y dibuja sus datos de vértices
	//
	void drawGL();

	void mainLoop();
};
