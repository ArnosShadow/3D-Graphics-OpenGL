#include "Shader.h"

void Shader::readSource()
{
	std::ifstream f(fileName);
	if (f.is_open()) {
		source = std::string(std::istreambuf_iterator<char>(f), {});
		std::cout << "Shader cargado: " << fileName << "\n";
	}
	else {
		std::cout << "ERROR: FICHERO NO ENCONTRADO " <<
			__FILE__ << ":" << __LINE__ << " " << fileName << "\n";
	}
}

void Shader::compileShader()
{
	const char* shaderCode = source.c_str();

	idShader = glCreateShader(type);
	if (idShader == 0) {
		std::cerr << "ERROR: No se pudo crear el shader: " << fileName << "\n";
		return;
	}
	glShaderSource(idShader, 1, &shaderCode, nullptr);
	glCompileShader(idShader);
}

void Shader::checkErrors()
{
	cout << idShader << endl;
	GLint fragment_compiled;
	glGetShaderiv(idShader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(idShader, 1024, &log_length, message);
		std::cout << "ERROR " << fileName << "\n" << message << "\n\n";
	}
}

void Shader::clean()
{
	glDeleteShader(idShader);
}
