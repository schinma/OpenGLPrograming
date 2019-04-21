#include "SkyBox.h"



SkyBox::SkyBox(ShaderProgram *shad) : _shader(shad)
{
	float side = 50.0f;   //size of cube
	float side2 = side / 2.0f;
	
	float v[24 * 3] = {
		// Front
		-side2, -side2, side2,
		side2, -side2, side2,
		side2,  side2, side2,
		-side2,  side2, side2,
		// Right
		side2, -side2, side2,
		side2, -side2, -side2,
		side2,  side2, -side2,
		side2,  side2, side2,
		// Back
		-side2, -side2, -side2,
		-side2,  side2, -side2,
		side2,  side2, -side2,
		side2, -side2, -side2,
		// Left
		-side2, -side2, side2,   //12
		-side2,  side2,  side2,   //13
		-side2,  side2, -side2,  //14
		-side2, -side2, -side2,  //15

		// Bottom
	   -side2, -side2, side2,
	   -side2, -side2, -side2,
		side2, -side2, -side2,
		side2, -side2, side2,
		// Top
		-side2,  side2, side2,
		 side2,  side2, side2,
		 side2,  side2, -side2,
		-side2,  side2, -side2
	};

	GLuint el[] = {
		0,2,1,    0,3,2,    4,6,5,     4,7,6,
		8,10,9,   8,11,10,  12,14,13,  12,15,14,
		16,18,17, 16,19,18, 20,22,21,  20,23,22
	};

	glGenVertexArrays(1, &vaoHandle);		
	glBindVertexArray(vaoHandle);			

	//create vbos for vertices
	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE,	0, 0);
	glEnableVertexAttribArray(0); // attrib number

	//IBO
	glGenBuffers(1, &iboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 4 * 3 * sizeof(GLuint), el, GL_STATIC_DRAW);

	glBindVertexArray(0);

	_texture.loadCubeMapTexture();
}


SkyBox::~SkyBox()
{
}

void SkyBox::draw()
{
	glUniform1i(_shader->uniform("DrawSkyBox"), GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture.getIdNumber(0));
	glUniform1i(_shader->uniform("CubeMapTex"), 0);

	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

}