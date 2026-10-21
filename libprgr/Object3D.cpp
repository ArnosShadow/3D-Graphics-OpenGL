#include "Object3D.h"

/*
vector4f position;
vector4f rotation;
vector4f scale;
matrix4x4f modelMatrix;
//Lista de vértices del objeto
std::vector<vertex_t> vertexList;
//Lista con los índices que se usarán para dibujar la lista de vértices
std::vector<int>idList;
*/

void Object3D::leerVertices(std::ifstream& f) {
	//repetir
	string linea;
	do {
		//leer linea
		std::getline(f, linea);
		//si no es un comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {

			//	//separar linea en identificador y posiciones
			std::stringstream l(linea); //Esto significa voy a iterar palabra por palabra
			string identificador;
			string posiciones;
			l >> identificador;
			l >> posiciones;
			std::vector<float> pos = splitString<float>(posiciones, ',');
			// //asignar posiciones a nuevo vertice.
			vertex_t v;
			v.posicion.x = pos[0];
			v.posicion.y = pos[1];
			v.posicion.z = pos[2];
			v.posicion.w = 1.0f;
			this->vertexList.push_back(v);
		}
		//hasta end
	} while (linea != "end");
}
void Object3D::leerColores(std::ifstream& f) {
	//repetir
	string linea;
	do {
		//leer linea
		std::getline(f, linea);
		//si no es un comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {

			//	//separar linea en identificador y posiciones
			std::stringstream l(linea); //Esto significa voy a iterar palabra por palabra
			string identificador;
			string colores;
			l >> identificador;
			l >> colores;
			std::vector<float> color = splitString<float>(colores, ',');
			// //asignar colores a nuevo vertice.
			// Sacar el identificador
			int vertexId = splitString<int>(identificador, ':')[0];

			this->vertexList[vertexId - 1].color = { color[0],color[1] ,color[2] ,color[3] };


		}
		//hasta end
	} while (linea != "end");
}
void Object3D::leerCaras(std::ifstream& f) {
	//repetir
	string linea;
	do {
		//leer linea
		std::getline(f, linea);
		//si no es un comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {

			//	//separar linea en identificador y posiciones
			std::stringstream l(linea); //Esto significa voy a iterar palabra por palabra
			string identificador;
			string vertexId;
			l >> identificador;
			l >> vertexId;
			std::vector<int> vIds = splitString<int>(vertexId, ',');
			// //asignar colores a nuevo vertice.
			// Sacar el identificador

			this->idList.push_back(vIds[0] - 1);
			this->idList.push_back(vIds[1] - 1);
			this->idList.push_back(vIds[2] - 1);

		}
		//hasta end
	} while (linea != "end");
}


void Object3D::leerNormales(std::ifstream& f)
{
	//repetir
	string linea = "";
	do {
		//	leer linea
		std::getline(f, linea);

		//si no es comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		{
			// // separar linea en identificador y posiciones
			std::stringstream l(linea);
			string identificador;
			string normal;
			l >> identificador;
			l >> normal;
			std::vector<float> n = splitString<float>(normal, ',');
			int vertexId = splitString<int>(identificador, ':')[0];
			this->vertexList[vertexId - 1].normal = { n[0],n[1],n[2],n[3] };
		}
		//hasta "end"
	} while (linea != "end");
}

void Object3D::leerTexturas(std::ifstream& f)
{
	//repetir
	string linea = "";
	do { //primer bucle: leer coordenadas de textura hasta "end"
		//	leer linea
		std::getline(f, linea);

		//si no es comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		{
			// // separar linea en identificador y posiciones
			std::stringstream l(linea);
			string identificador;
			string textureCoord;
			l >> identificador;
			l >> textureCoord;
			std::vector<float> tc = splitString<float>(textureCoord, ',');
			int vertexId = splitString<int>(identificador, ':')[0];
			this->vertexList[vertexId - 1].vTextureCoord = { tc[0],tc[1],-1,-1 };
		}
		//hasta "end"
	} while (linea != "end");

	do {//leer fichero de textura
		//	leer linea
		std::getline(f, linea);
		//si no es comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end"))
		{
			//fichero de textura en variable linea
			this->mat = new Material(linea);
		}
		//hasta "end"
	} while (linea != "end");
}


void Object3D::loadFromFile(string file)
{
	//Abrir fichero
	std::ifstream f(file);
	if (f.is_open()) {
		leerVertices(f);
		leerColores(f);
		leerNormales(f);
		leerTexturas(f);
		leerCaras(f);
		leerProgramas(f);
		//updateVolume();
		//Mientras no final de fichero 
		//	// leer vertices
		//	// leer colores
		//	// leer Caras

		//	leer una linea, 
		//		//Si comentario, skip
		//		//Si "end"  
		//
		//

	}
	else {
		cout << "ERROR: Fichero " << file << "no existe" << endl;
	}

}
Object3D::Object3D() {
	ObjectID = this->contador++;
	this->position = { 0,0,0,1 };
	this->rotation = { 0,0,0,1 };
	this->scale = { 1,1,1,1 };
	this->modelMatrix = identidad();


}

Object3D::Object3D(string file)
{
	ObjectID = contador++;
	this->position = { 0,0,0,1 };
	loadFromFile(file);


}
void Object3D::leerProgramas(std::ifstream& f) {
	//repetir
	std::vector<string> shaderList;
	string linea = "";
	do {
		//leer linea
		std::getline(f, linea);

		//si no es un comentario
		if ((linea[0] != '/' && linea[1] != '/') && (linea != "end")) {
			shaderList.push_back(linea);
			
		}
		//hasta end
	} while (linea != "end");
	this->program = new GLProgram();
	for (auto& file : shaderList) {
		program->addShader(file);
	}

	program->linkProgram();

	do {
		//    leer linea
		std::getline(f, linea);

		// si no es un comentario
		//Ha petado aqui
		if ((linea[0] != '/' && linea[1] != '/') && linea != "end") {
			vector<string> parts = splitString<string>(linea, '=');


			string name = parts[0];
			string value = parts[1];

			// Atributos
			if (name == "attributeVertPos") {
				this->program->shaderAttributes.attributeVertPos = value;
			}
			else if (name == "attributeVertColor") {
				this->program->shaderAttributes.attributeVertColor = value;
			}
			else if (name == "attributeVertNormal") {
				this->program->shaderAttributes.attributeVertNormal = value;
			}
			else if (name == "attributeVertTextureCoordinates") {
				this->program->shaderAttributes.attributeVertTextureCoordinates = value;
			}

			// Uniformes
			else if (name == "uniformMVP") {
				this->program->shaderUniforms.uniformMVP = value;
			}
			else if (name == "uniformModel") {
				this->program->shaderUniforms.uniformModel = value;
			}
			else if (name == "uniformLightPosition") {
				this->program->shaderUniforms.uniformLightPosition = value;
			}
			else if (name == "uniformLightColor") {
				this->program->shaderUniforms.uniformLightColor = value;
			}
			else if (name == "uniformLightIntensity") {
				this->program->shaderUniforms.uniformLightIntensity = value;
			}
			else if (name == "uniformLightType") {
				this->program->shaderUniforms.uniformLightType = value;
			}
			else if (name == "uniformLightDirection") {
				this->program->shaderUniforms.uniformLightDirection = value;
			}
			else if (name == "uniformSpecularConstant") {
				this->program->shaderUniforms.uniformSpecularConstant = value;
			}
			else if (name == "uniformDiffuseConstant") {
				this->program->shaderUniforms.uniformDiffuseConstant = value;
			}
			else if (name == "uniformMaterialShinny") {
				this->program->shaderUniforms.uniformMaterialShinny = value;
			}
			else if (name == "uniformCameraPosition") {
				this->program->shaderUniforms.uniformCameraPosition = value;
			}
			else if (name == "uniformTextureColor") {
				this->program->shaderUniforms.uniformTextureColor = value;
			}
			else if (name == "uniformNumLights") {
				this->program->shaderUniforms.uniformNumLights = value;
			}
			else if (name == "uniformIsOutline") {
				this->program->shaderUniforms.uniformIsOutline = value;
			}


		}
	} while (linea != "end");//hasta end

}
void Object3D::updatePhysics(float dt, const vector<BV*>& obstaculos)
{
	if (!volume || volume->type != sphere) return;

	SphereBV* esfera = static_cast<SphereBV*>(volume);

	esfera->velocity = esfera->velocity + esfera->aceleracion * dt;
	esfera->center = esfera->center+ esfera->velocity * dt;
	this->position = esfera->center;


	// Comprobar colisiones con obstáculos
	for (BV* obs : obstaculos) {
		if (obs == volume || !obs->Nodecorativo) continue;

		if (esfera->collision(obs)) {
			// Rebote en Y
			esfera->velocity.y *= -0.7f;

			// Ajustar la posición de la esfera justo encima del obstáculo
			float alturaObstaculo = obs->getCenter().y + obs->getSize().y / 2.0f;
			esfera->center.y = alturaObstaculo + esfera->radius;

			// Reajustar también la posición lógica del objeto
			this->position = esfera->center;

			break;
		}
	}

	updateModelMatrix();
}

void Object3D::updateVolume()
{
	//crear esfera
		//calcular radio
		//calcular centro de objeto

	//calcular máximos xyz
	//calcular mínimos xyz
	if (volume == nullptr) {

		vector4f max = { -numeric_limits<float>::max(),-numeric_limits<float>::max(),-numeric_limits<float>::max(),1 };
		vector4f min = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max(),1 };

		for (auto& v : vertexList)
		{
			max.x = max.x < v.posicion.x ? v.posicion.x : max.x;
			max.y = max.y < v.posicion.y ? v.posicion.y : max.y;
			max.z = max.z < v.posicion.z ? v.posicion.z : max.z;

			min.x = min.x > v.posicion.x ? v.posicion.x : min.x;
			min.y = min.y > v.posicion.y ? v.posicion.y : min.y;
			min.z = min.z > v.posicion.z ? v.posicion.z : min.z;
		}
		float textureW = max.x - min.x;
		float textureH = max.y - min.y;
		float pixelW = textureW / this->mat->texture->w;
		float pixelH = textureH / this->mat->texture->h;

		VBH* nuevo = new VBH();
		/*vector4f center = (max + min) / 2.0f;
		center.w = 1;
		float radious = 0.5;//distance(max, min) / 2.0f;

		BV* particle= new SphereBV(center, radious);

		this->collider = new VBH();
		this->collider->addParticle(particle);*/
		//por cada pixel que no es transparente
		for (int y = 0;y < this->mat->texture->h;y++)
			for (int x = 0;x < this->mat->texture->w;x++)
			{
				auto& p = this->mat->texture->pixels[y * this->mat->texture->w + x];
				//si no es transparente:
				if (p.a > 100)
				{
					vector4f center = { min.x + pixelW * (float)x + pixelW / 2.0f,
									   min.y + pixelH * (float)y + pixelH / 2.0f,
									   this->position.z,1 };
					float radius = (pixelW / 2.0f > pixelH / 2.0f) ?
						pixelW / 2.0f :
						pixelH / 2.0f;

					BV* particle = new SphereBV(center, radius);
					nuevo->addParticle(particle);
				}

			}
		nuevo->subdivide();


	}
	//actualizar nada más crear el collider
	auto mat = updateModelMatrix();
	this->volume->update(mat, this->size);
}

void Object3D::createTriangle()
{
	this->vertexList.push_back({ { 0,0.5,0,1 }, {1,0,0,1} });
	this->vertexList.push_back({ { -0.5,-0.5,0,1 }, { 0,0,1,1 } });
	this->vertexList.push_back({ { 0.5,-0.5,0,1 }, { 0,1,0,1 } });

	this->idList = {0,1,2};
}

void Object3D::move(double timeStep)
{/*
	this->rotation.y += 45.0 * timeStep;
	this->rotation.z += 45.0 * timeStep;
	if (EventManager::keyState[GLFW_KEY_Z]) {
		this->rotation.y += 45.0 *timeStep;
	}
	if (EventManager::keyState[GLFW_KEY_C]) {
		this->rotation.y -= 45.0 * timeStep;
	}
	if (EventManager::keyState[GLFW_KEY_Q]) {

		this->scale.x += 3 * timeStep;
		this->scale.y += 3 * timeStep;
		this->scale.z += 3 * timeStep;

	}
	if (EventManager::keyState[GLFW_KEY_E]) {

		this->scale.x -= 3 * timeStep;
		this->scale.y -= 3 * timeStep;
		this->scale.z -= 3 * timeStep;
	}

	if (EventManager::keyState[GLFW_KEY_RIGHT]) {

		this->position.x += 3 * timeStep;

	}
	if (EventManager::keyState[GLFW_KEY_LEFT]) {

		this->position.x -= 3 * timeStep;

	}
	if (EventManager::keyState[GLFW_KEY_SPACE]) {

		this->position.y += 3 * timeStep;

	}*/
	updateModelMatrix();
}

matrix4x4f Object3D::updateModelMatrix()
{
	this->modelMatrix = identidad() * matriz_translacion(this->position.x, this->position.y, this->position.z);
	this->modelMatrix = this->modelMatrix * matriz_rotacion(this->rotation.x, this->rotation.y, this->rotation.z);
	this->modelMatrix = this->modelMatrix * matriz_scale(this->scale.x, this->scale.y, this->scale.z);
	return modelMatrix;

}
