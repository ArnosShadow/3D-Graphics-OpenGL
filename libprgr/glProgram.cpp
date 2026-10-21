#include "glProgram.h"

GLProgram::GLProgram()
{
	//Crear programaId
	programId = glCreateProgram();
	/*vector<unsigned int> shaderIDList;
	//Por cada archivo
	for (auto& file : shaderList) {
		//check tipo de shader 
		unsigned int shaderType = -1;
		if (file.ends_with(".vert")) { //Shader de vertices
			shaderType = GL_VERTEX_SHADER;
		}
		else if (file.ends_with(".frag")) {
			shaderType = GL_FRAGMENT_SHADER;
		}
		else {
			std::cerr << "ERROR: Tipo de shader desconocido en " << file << std::endl;
			continue;
		}

		addShader(file);
		
		linkProgram();
	}
	
	//Lista de variables activas, en la GPU tenemos una zona de variables muy limitada.
	readVarList();*/
}


void GLProgram::addShader(std::string fileName)
{
	Shader* shader = new Shader(fileName);
	//atar el programa
	glAttachShader(programId, shader->idShader);
	shaderList.push_back(shader);
}

void GLProgram::linkProgram()
{
	//linkar programa
	glLinkProgram(programId);
	//check error de programa
	checkProgramError();
	//liberar memoria de shaders.
	for (auto shader : shaderList) {
		glDetachShader(programId, shader->idShader);
		shader->clean();
	}

	//Lista de variables activas, en la GPU tenemos una zona de variables muy limitada.
	readVarList();
}



void GLProgram::clean()
{

	for (auto shader : shaderList) {
		delete shader;
	}
	shaderList.clear();

	glDeleteProgram(programId);
}

void GLProgram::checkProgramError()
{
	GLint program_linked;
	glGetProgramiv(programId, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(programId, 1024, &log_length, message);
		std::cout << "ERROR \n" << message << "\n\n";
	}
}

void GLProgram::readVarList()
{
	int numAttributes = 0;
	int numUniforms = 0;
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	for (int i = 0; i < numAttributes; i++)
	{
		char varName[100];
		int bufSize = 100, length = 0, size = 0;
		GLenum type = -1;
		glGetActiveAttrib(programId, (GLuint)i, bufSize, &length, &size, &type, varName);
		varList[std::string(varName)] = glGetAttribLocation(programId, varName);
	}
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numUniforms);
	for (int i = 0; i < numUniforms; i++)
	{
		string varName; varName.resize(100);
		int bufSize = 100, length = 0, size = 0;
		GLenum type = -1;
		glGetActiveUniform(programId, (GLuint)i, bufSize, &length, &size, &type, varName.data());
		varName = std::string(varName.c_str()); //interrogar con nombre
		if (varName[varName.length() - 1] == ']') {//si es de tipo array
			std::string arrName = varName.substr(0, varName.find('['));
			for (int i = 0; i < size; i++) //coneguir la lista completa de nombres
			{
				std::string arrNameIdx = arrName + "[" + std::to_string(i) + "]";
				varList[arrNameIdx] = glGetUniformLocation(programId, arrNameIdx.c_str());
			}
		}
		else
			varList[varName] = glGetUniformLocation(programId, varName.c_str());
	}
}

void GLProgram::use()
{
	glUseProgram(programId);
}



void GLProgram::setAttributeData(string nombre,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{	
	if (varList.find(nombre) == varList.end()) {
		cout << "Ha petado: Variable shader " << nombre << "no encontrado" << endl;
	}
	else {
		unsigned int index = varList[nombre];
		glEnableVertexAttribArray(index);
		//No se pueden subir mas de 4.
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	
	}

}

void GLProgram::setUniformData(dataType_e tipo, void* dato, string nombre) {
	if (varList.find(nombre) == varList.end()) {
		cout << "Ha petado: Variable shader " << nombre << " no encontrado" << endl;
		return;
	}

	switch (tipo)
	{
	case matrix4:
		glUniformMatrix4fv(varList[nombre], 1, GL_TRUE, (float*)dato);
		break;
	case vector4:
		glUniform4fv(varList[nombre], 1, (float*)dato);
		break;
	case floatpoint:
		glUniform1f(varList[nombre], *(float*)dato);
		break;
	case integer:
		glUniform1i(varList[nombre], *(int*)dato);
		break;
	default:
		cout << "Ha petado: tipo de dato no soportado" << endl;
		break;
	}
}

void GLProgram::setUniformDataArray(dataType_e tipo, void* datos, int count, string nombre) {
	std::string nombreArray = nombre + "[0]";

	if (varList.find(nombreArray) == varList.end()) {
		std::cout << "Ha petado: Variable shader " << nombreArray << " no encontrado\n";
		return;
	}

	switch (tipo) {
	case vector4:
		glUniform4fv(varList[nombreArray], count, (float*)datos);
		break;
	case floatpoint:
		glUniform1fv(varList[nombreArray], count, (float*)datos);
		break;
	case integer:
		glUniform1iv(varList[nombreArray], count, (int*)datos);
		break;
	default:
		std::cout << "Ha petado: tipo de dato no soportado para array\n";
		break;
	}
}

GLProgram::~GLProgram()
{
	clean();

}
