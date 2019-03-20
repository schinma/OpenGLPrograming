#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"
#include <glm/glm.hpp>

typedef struct _param {
	float X_translation;
	float Y_translation = 10.0f;
	float Z_translation;
	float Radius = 1.0f;
	float Rotation;
	int LightNumber = 5;
} Parameter;

typedef struct _Material
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat shininess;

} Material;

typedef struct _Light
{
	glm::vec3 Intensity;
	glm::vec4 Position;
	glm::vec4 Direction;
	float cutOff;
	float innerCutOff;
	float exponent;

} Light;


#endif /* MATERIAL_H */