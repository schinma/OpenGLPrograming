#include "Shader.h"
#include <iostream>


Shader::Shader(const std::string &name) : m_name(name)
{
	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles(name + ".vert", name + ".frag");
}


Shader::~Shader()
{
	delete(m_shaderProgram);
}

ShaderProgram *Shader::getShaderProgram()
{
	return m_shaderProgram;
}