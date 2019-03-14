#ifndef COLOR_CUBE_H
#define COLOR_CUBE_H

#include <iostream>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "Loader.h"
#include "ModelView.h"
#include "IObject.h"

class ColorCube : public IObject {

private:
	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_normals;
	std::vector<glm::vec3> cube_normal;
	std::vector<glm::vec3> cube_vertices;
	Model *_model;

	void setup();
	glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

public:
	ColorCube();
	void draw();

};


#endif
