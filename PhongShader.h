#pragma once

#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "Shader.h"


class PhongShader :
	public Shader
{
public:
	PhongShader();
	~PhongShader();

	void addUniforms() override;
	void setUniforms(glm::mat4 view, Parameter param);
};

#endif /* PHONG_SHADER_H */