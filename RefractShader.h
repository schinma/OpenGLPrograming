#pragma once


#include "Shader.h"

class RefractShader : public Shader
{
public:
	RefractShader(const std::string &name) : Shader("shaders/"+name) {}
	~RefractShader() {}

	void addUniforms() override
	{
		m_shaderProgram->addUniform("mvp");
		m_shaderProgram->addUniform("CubeMapTex");
		m_shaderProgram->addUniform("DrawSkyBox");

		m_shaderProgram->addUniform("DrawSkyBox");
		m_shaderProgram->addUniform("WorldCameraPosition");    //Camera position in the world space

		m_shaderProgram->addUniform("ModelMatrix");  //Object color
	
		m_shaderProgram->addUniform("EtaR");
		m_shaderProgram->addUniform("EtaG");
		m_shaderProgram->addUniform("EtaB");
	}
};
