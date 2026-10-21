#pragma once
#include "common.h"
#include "Shader.h"


using namespace std;
class GLProgram {

public:
	typedef enum {
		integer,
		floatpoint,
		vector4,
		matrix4
	
	} dataType_e;

	typedef struct {
		std::string attributeVertPos;
		std::string attributeVertColor;
		std::string attributeVertNormal;
		std::string attributeVertTextureCoordinates;
	} attribute_t;

	typedef struct {
		std::string uniformMVP;
		std::string uniformModel;
		std::string uniformLightPosition;
		std::string uniformLightColor;
		std::string uniformLightIntensity;
		std::string uniformLightType;
		std::string uniformLightDirection;
		std::string uniformSpecularConstant;
		std::string uniformDiffuseConstant;
		std::string uniformMaterialShinny;
		std::string uniformCameraPosition;
		std::string uniformTextureColor;
		std::string uniformNumLights;
		std::string uniformIsOutline;
	} uniform_t;


	attribute_t shaderAttributes;
	uniform_t shaderUniforms;

	unsigned int programId = -1;
	map<string, unsigned int> varList;
	std::vector<Shader*> shaderList;            


	GLProgram();
	//Método que, a partir de un nombre de fichero, crea un objeto de tipo Shader
	//con los datos de ese fichero.Una vez cargado, almacenará ese objeto en la
	//lista “shaderList”
	void addShader(std::string fileName);
		
	void linkProgram();
	
	void clean();
	
	void checkProgramError();

	void readVarList(); //lee variables de shaders
	void use(); //activa el uso de este programa
	void setAttributeData(string nombre,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void setUniformData(dataType_e tipo, void* dato, string nombre);

	void setUniformDataArray(dataType_e tipo, void* datos, int count, string nombre);

	//Destruir el programId
	~GLProgram();

};
