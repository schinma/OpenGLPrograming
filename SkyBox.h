#pragma once

#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <string>
#include <iostream>
#include <vector>
#include "GL/glew.h" // Contains all the necessery OpenGL #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Loader.h"
#include "IObject.h"
#include "Texture.h"

class SkyBox : public IObject
{

private:
	unsigned int vaoHandle, vboHandle, iboHandle;
	Texture _texture;
	ShaderProgram *_shader;

public:
	SkyBox(ShaderProgram *shader);
	~SkyBox();

	void draw();
};

#endif /* SKY_BOX_H */