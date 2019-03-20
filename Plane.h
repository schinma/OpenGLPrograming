#pragma once

#ifndef PLANE_HPP
#define PLANE_HPP

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

#include <vector>
#include <iostream>

#include "IObject.h"

class Plane : public IObject
{
public:
	Plane(int size, int numcell);

	void setup(float size, int nSquares);
	void draw();
	void resize(int size, int numcell);
	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;

	int size;
	int ncell;
};

#endif