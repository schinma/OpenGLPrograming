#include "Shader.h"



Shader::Shader(std::string &name) : _name(name)
{
	_shaderProgram = new ShaderProgram();
	_shaderProgram->initFromFiles(name + ".vert", name + ".frag");
}


Shader::~Shader()
{
}
