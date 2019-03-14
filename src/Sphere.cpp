#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Sphere::Sphere()
{

}


Sphere::~Sphere()
{
}



Sphere::Sphere(float rad, GLuint sl, GLuint st) :
radius(rad), slices(sl), stacks(st)
{

	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];  
	// Tex coords
	float * tex = new float[2 * nVerts];    //we don't use it now
	// Index
	unsigned int * el = new unsigned int[elements];  //index

	// Generate the vertex data
	generateVerts(v, n, tex, el);
	

	//create vao, vbo and ibo here... (We didn't use std::vector here...)
	setup(v, n, el);

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;

}

void Sphere::setup(float *vertices, float *normals, unsigned int *el) 
{
	glGenVertexArrays(1, &VAO);		//create a vao
	glBindVertexArray(VAO);			//"activate" or "im going to do somthing on here" 	

	//create vbos for vertices
	glGenBuffers(1, &VBO_position);		//
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);  //bind the vbo
	//create a buffer in GPU memory and copy the data
	glBufferData(GL_ARRAY_BUFFER, nVerts * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,				//attribute number
		3,				//number of data for each vertex
		GL_FLOAT,		//data type
		GL_FALSE,		//is it normalized ?
		0,				//stride : gap between two adjacent vertex 
		0				//offset : gap from the starting address 
	);
	glEnableVertexAttribArray(0); // attrib number


	//create vbos for normals 
	glGenBuffers(1, &VBO_normal);		//
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);  //bind the vbo
	//create a buffer in GPU memory and copy the data
	glBufferData(GL_ARRAY_BUFFER, nVerts * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,				//attribute number
		3,				//number of data for each vertex
		GL_FLOAT,		//data type
		GL_FALSE,		//is it normalized ?
		0,				//stride : gap between two adjacent vertex 
		0				//offset : gap from the starting address 
	);
	glEnableVertexAttribArray(1); // attrib number

	//IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);			//unbinding


}

void Sphere::draw() 
{
	glBindVertexArray(VAO);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	GLuint * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

int Sphere::getVertexArrayHandle() 
{
	return this->VAO;
}

