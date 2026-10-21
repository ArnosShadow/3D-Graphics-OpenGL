#include <iostream>
#include <vector>
#include "vertex.h"
#include "EventManager.h"
#include "glProgram.h"
#include "Material.h"
#include "BV.h"
using namespace libPRGR;


class Object3D {
public:
	int ObjectID;
	inline static int contador=0;
	Material* mat;
	vector4f position;
	vector4f rotation;
	vector4f scale = { 1,1,1,1 };
	matrix4x4f modelMatrix;
	//Lista de vértices del objeto
	std::vector<vertex_t> vertexList;
	//Lista con los índices que se usarán para dibujar la lista de vértices
	std::vector<unsigned int>idList;
	BV* volume;
	vector4f size = { 1, 1, 1, 0 };
	bool Nodecorativo= true;


	GLProgram* program = nullptr;

	void leerVertices(std::ifstream& f);

	void leerColores(std::ifstream& f);

	void leerCaras(std::ifstream& f);

	void leerNormales(std::ifstream& f);

	void leerTexturas(std::ifstream& f);

	void loadFromFile(string file);

	Object3D();

	
	Object3D(string file);

	void leerProgramas(std::ifstream& f);

	void updateVolume();
	void updatePhysics(float dt, const vector<BV*>& obstaculos);
	void createTriangle();
	virtual void move(double timeStep);
	matrix4x4f updateModelMatrix();
};