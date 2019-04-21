#pragma once

#ifndef NORMAL_MAP_SHADER_H
#define NORMAL_MAP_SHADER_H

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

class NormalMapShader :
	public Shader
{
public:
	NormalMapShader();
	~NormalMapShader();

	void addUniforms() override;
	void setUniforms(glm::mat4 view, Parameter param);
private:

	int nb_Lights;
};

#endif /* NORMAL_MAP_SHADER_H */