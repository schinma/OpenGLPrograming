#ifndef ___SPHERE_H
#define ___SPHERE_H

#include "GL/glew.h"
#include <GL/GL.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

#include "Loader.h"
#include "IObject.h"
#include "Texture.h"

class Sphere : public IObject
{
public:
	Sphere();
	Sphere(float rad, GLuint sl, GLuint st, ShaderProgram *shader);
	~Sphere();

	void setup(float *v, float *n, unsigned int * el, float *texture);
	void draw();
	int getVertexArrayHandle();

	GLuint VAO, VBO_position, VBO_normal, IBO, VBO_texture;

private:
	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

	Texture texture;
	ShaderProgram *_shader;

	void generateVerts(float *, float *, float *, GLuint *);
};


#endif
