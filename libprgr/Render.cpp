#include "Render.h"
#include "OrbitalLight.h"



 Render::Render(int alto, int ancho) : alto(alto), ancho(ancho) {
	initGL(alto, ancho);
}

// Activará un pixel(pondrá el valor 1) en la posición "x,y" de la matriz

 void Render::PutPixel(int x, int y) {
	int posicionX = centroX + x;
	int posicionY = centroY - y;

	if (posicionX < ancho && posicionY < alto && posicionX >= 0 && posicionY >= 0) {
		buffer[posicionY][posicionX] = '1';
	}

}

// Limpiará el buffer actual(pondrá un 0 en todas las posiciones de la matriz)

 void Render::ResetBufer() {
	/*for (int i = 0; i < alto; i++) {
	for (int j = 0; j < ancho; j++) {
	MODIFICAR;
	}
	}*/
}

 GLFWwindow* Render::initGL(int horizontal, int vertical) {
	if (glfwInit() != GLFW_TRUE) {
		cout << "ERROR GLFW\n";
	}
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	window = glfwCreateWindow(horizontal, vertical, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	EventManager::init(window);
	glEnable(GL_DEPTH_TEST);
	return window;

}


 //Añade camara al sistema
 void Render::putCamera(Camera* camj) {
	 this->camera = camj;
 }
//
//Método que añade un objeto a la lista de objetos.Debe crear su bufferObject y
//almacenar en el mapa bufferObjets los identificadores de buffers creados / cargados en
//GPU
//


 void Render::setupObject(Object3D* obj) {
 
	 bufferObject bo;
	 //Generar buffers de datos
	 glGenVertexArrays(1, &bo.idArray);
	 glGenBuffers(1, &bo.idVertexArray);
	 glGenBuffers(1, &bo.idIndexArray);


	 //por cada buffer
	 glBindVertexArray(bo.idArray);
	 //activar cada buffer
	 glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
	 //subir por cada buffer sus datos
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * obj->vertexList.size(), obj->vertexList.data(), GL_STATIC_DRAW);
	 //activar cada buffer
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);
	 //subir por cada buffer sus datos
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj->idList.size(), obj->idList.data(), GL_STATIC_DRAW);
	 //guardar bufferObject en mapa
	 bufferObjects[obj->ObjectID] = bo;
 }

void Render::putObject(Object3D* obj) {
	setupObject(obj);
	objectList.push_back(obj);
}

//


void Render::putLight(Light* light) {
	this->lights.push_back(light);
}

 void Render::removeObject(Object3D* obj) {
	auto iterador = bufferObjects.find(obj->ObjectID);

	//Coge el segundo valor del map dicho iterador.
	bufferObject& bo = iterador->second;

	// 1 significa que se borra 1 y no mas.
	glDeleteBuffers(1, &bo.idVertexArray);
	glDeleteBuffers(1, &bo.idIndexArray);
	glDeleteVertexArrays(1, &bo.idArray);

	bufferObjects.erase(iterador);

	auto objIt = find(objectList.begin(), objectList.end(), obj);
	if (objIt != objectList.end()) {
		objectList.erase(objIt);
	}
}

//
//	Método que dibuja los objetos en la ventana de openGL.Este método recorre la lista de
//	objetos, y por cada uno de ellos los carga en GPU y dibuja sus datos de vértices
//

 void Render::drawGL() {

	 for (auto obj : objectList) {
		 matrix4x4f model = obj->updateModelMatrix();
		 auto view = camera->computeViewMatrix();
		 auto projection = camera->computeProjectionMatrix();

		 matrix4x4f MVP = projection * view * model;
		 //activar buffers
		 auto bo = bufferObjects[obj->ObjectID];
		 glBindVertexArray(bo.idArray);
		 glBindBuffer(GL_ARRAY_BUFFER, bo.idVertexArray);
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.idIndexArray);

		 // Activar programa
		 obj->program->use();

		 // Copiar datos de matriz MVP
		 obj->program->setUniformData(GLProgram::matrix4, MVP.mat1, obj->program->shaderUniforms.uniformMVP);
		 obj->program->setUniformData(GLProgram::matrix4, model.mat1, obj->program->shaderUniforms.uniformModel);

		 // Setear atributos
		 obj->program->setAttributeData(obj->program->shaderAttributes.attributeVertPos, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, posicion));
		 //obj->program->setAttributeData(obj->program->shaderAttributes.attributeVertColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, color));
		 obj->program->setAttributeData(obj->program->shaderAttributes.attributeVertNormal, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
		 obj->program->setAttributeData(obj->program->shaderAttributes.attributeVertTextureCoordinates, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, vTextureCoord));

		 // Procesar luces
		 int numLights = lights.size();
		 std::vector<vector4f> pos, dir, color;
		 std::vector<float> Iks;
		 std::vector<int> types;

		 for (auto* light : lights) {
			 pos.push_back(light->pos);
			 dir.push_back(light->direction);
			 color.push_back(light->color);
			 Iks.push_back(light->I);
			 types.push_back(light->type);
		 }

		 obj->program->setUniformData(GLProgram::integer, &numLights, obj->program->shaderUniforms.uniformNumLights);
		 obj->program->setUniformDataArray(GLProgram::vector4, pos.data(), numLights, obj->program->shaderUniforms.uniformLightPosition);
		 obj->program->setUniformDataArray(GLProgram::vector4, dir.data(), numLights, obj->program->shaderUniforms.uniformLightDirection);
		 obj->program->setUniformDataArray(GLProgram::vector4, color.data(), numLights, obj->program->shaderUniforms.uniformLightColor);
		 obj->program->setUniformDataArray(GLProgram::floatpoint, Iks.data(), numLights, obj->program->shaderUniforms.uniformLightIntensity);
		 obj->program->setUniformDataArray(GLProgram::integer, types.data(), numLights, obj->program->shaderUniforms.uniformLightType);

		 // Paso 1: Dibujar contorno negro
		 int uno = 1, cero = 0;
		 glCullFace(GL_FRONT);
		 glEnable(GL_CULL_FACE);
		 glDrawElements(GL_TRIANGLES, obj->idList.size(), GL_UNSIGNED_INT, nullptr);
		 obj->program->setUniformData(GLProgram::integer, &uno, obj->program->shaderUniforms.uniformIsOutline);

		 // Paso 2: Dibujar objeto normal
		 glCullFace(GL_BACK);
		 glDrawElements(GL_TRIANGLES, obj->idList.size(), GL_UNSIGNED_INT, nullptr);
		 obj->program->setUniformData(GLProgram::integer, &cero, obj->program->shaderUniforms.uniformIsOutline);

		 // Material y cámara
		 obj->program->setUniformData(GLProgram::floatpoint, &(obj->mat->kd), obj->program->shaderUniforms.uniformDiffuseConstant);
		 obj->program->setUniformData(GLProgram::floatpoint, &(obj->mat->ks), obj->program->shaderUniforms.uniformSpecularConstant);
		 obj->program->setUniformData(GLProgram::integer, &(obj->mat->shinny), obj->program->shaderUniforms.uniformMaterialShinny);
		 obj->program->setUniformData(GLProgram::vector4, &(camera->pos), obj->program->shaderUniforms.uniformCameraPosition);

		 // Activar textura
		 int textureUnit = 0;
		 obj->mat->bind(textureUnit);
		 obj->program->setUniformData(GLProgram::integer, &textureUnit, obj->program->shaderUniforms.uniformTextureColor);


		 glDrawElements(GL_TRIANGLES, obj->idList.size(), GL_UNSIGNED_INT, nullptr);

	 }
 }

 void Render::mainLoop() {
	double FPSDesire = 1.0 / (120.0f);
	double ultimo = glfwGetTime();
	double deltaTime = 0, actual = 0;
	std::vector<BV*> obstaculos;
	while (!glfwWindowShouldClose(window)) {
		actual = glfwGetTime();
		double deltaTime = (actual - ultimo);
		ultimo = actual;

		//std::cout << deltaTime << std::endl;
		for (auto obj : objectList)
		{
			obj->move(deltaTime); 
			obj->updateModelMatrix();
			//light->posicionObjeto = obj->position;
		}

		vector4f originalPos = camera->pos;
		camera->move();

		SphereBV camVolume(camera->pos, 0.2f, true);
		
		for (auto* obj : objectList) {
			if (obj->volume && obj->volume->Nodecorativo)
				obstaculos.push_back(obj->volume);
		}

		// Aplicar física a esferas
		for (auto obj : objectList) {
			obj->updatePhysics(deltaTime, obstaculos);
		}

		for (auto* obstaculo : obstaculos) {
			if (camVolume.collision(obstaculo)) {
				camera->pos = originalPos;
				break;
			}
		}
		
		for (auto light : lights) {
			if (light->type == Light::puntoLuz) {
				light->move(deltaTime);
			}
		}


		// dibujar
		//limpiar buffers
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//mandar triángulos

		drawGL();

		// check eventos
		glfwPollEvents();

		// cambiarBuffers
		glfwSwapBuffers(window);

		double sleepT = FPSDesire - deltaTime;//tiempo que sobra entre FPS
		if (sleepT>0)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(sleepT * 1000.0f)));

	}
}
