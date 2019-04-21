#include "NormalMapShader.h"



NormalMapShader::NormalMapShader() : Shader("shaders/normal_map"), nb_Lights(1)
{
}


NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::addUniforms()
{
	m_shaderProgram->addUniform("mvp");

	m_shaderProgram->addUniform("Ka");  //Ambient Object Color : vec3
	m_shaderProgram->addUniform("Ks");  //Specular Object Color : vec3
	m_shaderProgram->addUniform("shininess");
	m_shaderProgram->addUniform("ModelViewMatrix"); //View*Model : mat4
	m_shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3

	m_shaderProgram->addUniform("ColorTex");
	m_shaderProgram->addUniform("NormalMapTex");

	for (int i = 0; i < nb_Lights; i++) {
		std::string name, name1, name2, name3, name4, name5, name6;
		name = "Light[";
		name1 = name + std::to_string(i) + "].Position";
		name2 = name + std::to_string(i) + "].Intensity";
		name3 = name + std::to_string(i) + "].Direction";
		m_shaderProgram->addUniform(name1);
		m_shaderProgram->addUniform(name2);
		m_shaderProgram->addUniform(name3);
	}
}

void NormalMapShader::setUniforms(glm::mat4 view, Parameter param)
{
	// set the lights parameters
	glUniform3fv(m_shaderProgram->uniform("Light[0].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.8f, 0.8f)));
	
	glm::vec4 lightPos(param.X_translation, param.Y_translation, param.Z_translation, 1);

	glm::vec4 viewPos = view * lightPos;
	glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;

	for (int i = 0, theta = param.Rotation; i < nb_Lights; i++, theta += 72) {
		float x = param.Radius * cos(theta*(glm::pi<float>() / 180)) + lightPos.x;
		float z = param.Radius * sin(theta*(glm::pi<float>() / 180)) + lightPos.z;
		glm::vec4 newPos = glm::vec4(x, lightPos.y, z, 1);
		glm::vec4 viewPos = view * newPos;
		glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;
		glUniform4fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(viewPos));
		glUniform4fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Direction"), 1, glm::value_ptr(direction));
	}
}
