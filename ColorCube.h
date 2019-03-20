#ifndef COLOR_CUBE_H
#define COLOR_CUBE_H

#include <iostream>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "Loader.h"
#include "ModelView.h"
#include "IObject.h"
#include "Texture.h"
#include "Shader.h"

class ColorCube : public IObject {

private:
	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_normals, vbo_textures;
	std::vector<glm::vec3> cube_normal;
	std::vector<glm::vec3> cube_vertices;
	ShaderProgram *_shader;
	Texture _texture;

	void setup();
	glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

public:
	ColorCube(ShaderProgram *shader);
	void draw() override;
	void setShader(ShaderProgram *shader);
};


#endif /* COLOR_CUBE */
