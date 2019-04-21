#pragma once

#ifndef SKY_BOX_SHADER__H
#define SKY_BOX_SHADER_H

#include "Shader.h"

class SkyBoxShader : public Shader
{
public:
	SkyBoxShader(const std::string &name) : Shader("shaders/"+name) {}
	~SkyBoxShader() {}

	void addUniforms() override
	{
		m_shaderProgram->addUniform("mvp");
		m_shaderProgram->addUniform("CubeMapTex");
		m_shaderProgram->addUniform("DrawSkyBox");

		m_shaderProgram->addUniform("DrawSkyBox");
		m_shaderProgram->addUniform("WorldCameraPosition");    //Camera position in the world space

		m_shaderProgram->addUniform("ModelMatrix");  //Model Matrix for converting from local to world
		m_shaderProgram->addUniform("MaterialColor");  //Object color
		m_shaderProgram->addUniform("ReflectFactor");  //Ratio of mixup the objectcolor with cubemap color

	}
};


#endif // !SHADER_SKY_BOX_H
