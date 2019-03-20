#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "Loader.h"
#include <glm/glm.hpp>
#include "Material.h"

class Shader
{
public:
	Shader(const std::string &name);
	~Shader();

	virtual void addUniforms() = 0;
	ShaderProgram *getShaderProgram();

protected:
	ShaderProgram *m_shaderProgram;
	std::string m_name;
};

#endif /*Shader*/