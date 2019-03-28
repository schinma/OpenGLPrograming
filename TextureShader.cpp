#include "TextureShader.h"



TextureShader::TextureShader() : Shader("shaders/texture")
{
}


TextureShader::~TextureShader()
{
}

void TextureShader::addUniforms()
{
	m_shaderProgram->addUniform("mvp");
	m_shaderProgram->addUniform("Kd");  //Diffuse Object Color :vec3
	m_shaderProgram->addUniform("Ka");  //Ambient Object Color : vec3
	m_shaderProgram->addUniform("Ks");  //Specular Object Color : vec3
	m_shaderProgram->addUniform("shininess");
	m_shaderProgram->addUniform("ModelViewMatrix"); //View*Model : mat4
	m_shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3

	m_shaderProgram->addUniform("ColorTex");
	m_shaderProgram->addUniform("Tex2");

	for (int i = 0; i < 5; i++) {
		std::string name, name1, name2, name3;
		name = "Light[";
		name1 = name + std::to_string(i) + "].Position";
		name2 = name + std::to_string(i) + "].Intensity";
		name3 = name + std::to_string(i) + "].Direction";
		m_shaderProgram->addUniform(name1);
		m_shaderProgram->addUniform(name2);
		m_shaderProgram->addUniform(name3);
	}
}

void TextureShader::setUniforms(glm::mat4 view, Parameter param)
{
	// set the "material" parameter

	// set the lights parameters
	glUniform3fv(m_shaderProgram->uniform("Light[0].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.8f, 0.8f)));
	glUniform3fv(m_shaderProgram->uniform("Light[1].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.8f)));
	glUniform3fv(m_shaderProgram->uniform("Light[2].Intensity"), 1, glm::value_ptr(glm::vec3(0.8f, 0.0f, 0.0f)));
	glUniform3fv(m_shaderProgram->uniform("Light[3].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.8f, 0.0f)));
	glUniform3fv(m_shaderProgram->uniform("Light[4].Intensity"), 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));

	glm::vec4 lightPos(param.X_translation, param.Y_translation, param.Z_translation, 1);

	glm::vec4 viewPos = view * lightPos;
	glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;

	for (int i = 0, theta = param.Rotation; i < 5; i++, theta += 72) {
		float x = param.Radius * cos(theta*(glm::pi<float>() / 180)) + lightPos.x;
		float z = param.Radius * sin(theta*(glm::pi<float>() / 180)) + lightPos.z;
		glm::vec4 newPos = glm::vec4(x, lightPos.y, z, 1);
		glm::vec4 viewPos = view * newPos;
		glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;
		glUniform4fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(viewPos));
		glUniform4fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Direction"), 1, glm::value_ptr(direction));
	}
}
