#pragma once

#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

class TextureShader :
	public Shader
{
public:
	TextureShader();
	~TextureShader();

	void addUniforms() override;
	void setUniforms(glm::mat4 view, Parameter param);
};

#endif /* TEXTURE_SHADER_H */