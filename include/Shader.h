#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "Loader.h"

class Shader
{
public:
	Shader(std::string &name);
	~Shader();

	virtual void createUniforms() = 0;
	virtual void setUniforms() = 0;

private:
	ShaderProgram *_shaderProgram;
	std::string _name;
};

#endif /*Shader*/