#include "ColorCube.h"


ColorCube::ColorCube(ShaderProgram *shader) : _shader(shader)
{
	setup();
}

glm::vec3 ColorCube::computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;

	glm::vec3 normal = glm::cross(v1, v2);

	return (normal);
}

void ColorCube::setup()
{
	//setup texture
	_texture.loadTexture("brick1.jpg", "moss.png");

	// Generate the vertices

	//front
	cube_vertices.emplace_back(-1.0, -1.0, 1.0);
	cube_vertices.emplace_back(1.0, -1.0, 1.0);
	cube_vertices.emplace_back(1.0, 1.0, 1.0);

	cube_vertices.emplace_back(1.0, 1.0, 1.0);
	cube_vertices.emplace_back(-1.0, 1.0, 1.0);
	cube_vertices.emplace_back(-1.0, -1.0, 1.0);

	//top
	cube_vertices.emplace_back(-1.0, 1.0, 1.0);
	cube_vertices.emplace_back(1.0, 1.0, 1.0);
	cube_vertices.emplace_back(-1.0, 1.0, -1.0);

	cube_vertices.emplace_back(-1.0, 1.0, -1.0);
	cube_vertices.emplace_back(1.0, 1.0, 1.0);
	cube_vertices.emplace_back(1.0, 1.0, -1.0);

		//bottom
	cube_vertices.emplace_back(-1.0, -1.0, 1.0);
	cube_vertices.emplace_back(-1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, -1.0, 1.0);


	cube_vertices.emplace_back(-1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, -1.0, 1.0);

		//right
	cube_vertices.emplace_back(1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, 1.0, 1.0);
	cube_vertices.emplace_back(1.0, -1.0, 1.0);

	cube_vertices.emplace_back(1.0, 1.0, 1.0);
	cube_vertices.emplace_back(1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, 1.0, -1.0);

		//left
	cube_vertices.emplace_back(-1.0, -1.0, -1.0);
	cube_vertices.emplace_back(-1.0, -1.0, 1.0);
	cube_vertices.emplace_back(-1.0, 1.0, 1.0);

	cube_vertices.emplace_back(-1.0, 1.0, 1.0);
	cube_vertices.emplace_back(-1.0, 1.0, -1.0);
	cube_vertices.emplace_back(-1.0, -1.0, -1.0);

		// back
	cube_vertices.emplace_back(-1.0, -1.0, -1.0);
	cube_vertices.emplace_back(1.0, 1.0, -1.0);
	cube_vertices.emplace_back(1.0, -1.0, -1.0);

	cube_vertices.emplace_back(1.0, 1.0, -1.0);
	cube_vertices.emplace_back(-1.0, -1.0, -1.0);
	cube_vertices.emplace_back(-1.0, 1.0, -1.0);


	float text[36 * 2] = {
		//front
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		1.0f, 0.0f, 
		0.0f, 0.0f, 
		0.0f, 1.0f,
		
		//top
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		
		//bottom
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		//right
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		//left
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		//back
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};	

	// Calculate the normals for each vertices

	for (int j = 0; j < cube_vertices.size(); j += 3) {

		glm::vec3 normal = computeNormal(cube_vertices[j], cube_vertices[j + 1], cube_vertices[j + 2]);
		for (int i = 0; i < 3; i++)
			cube_normal.emplace_back(normal);
	}


	glGenVertexArrays(1, &vaoHandle);		//create a vao
	glBindVertexArray(vaoHandle);			//"activate" or "im going to do somthing on here" 	

	//create vbos for vertices
	glGenBuffers(1, &vbo_cube_vertices);		//
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);  //bind the vbo
	//create a buffer in GPU memory and copy the data
	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * 3 * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);
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
	glGenBuffers(1, &vbo_cube_normals);		//
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);  //bind the vbo
	//create a buffer in GPU memory and copy the data
	glBufferData(GL_ARRAY_BUFFER, cube_normal.size() * 3 * sizeof(float), cube_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,				//attribute number
		3,				//number of data for each vertex
		GL_FLOAT,		//data type
		GL_FALSE,		//is it normalized ?
		0,				//stride : gap between two adjacent vertex 
		0				//offset : gap from the starting address 
	);
	glEnableVertexAttribArray(1); // attrib number

	glGenBuffers(1, &vbo_textures);		//
	glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);  //bind the vbo
	//create a buffer in GPU memory and copy the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32 * 2, text, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,				//attribute number
		2,				//number of data for each vertex
		GL_FLOAT,		//data type
		GL_FALSE,		//is it normalized ?
		0,				//stride : gap between two adjacent vertex 
		0				//offset : gap from the starting address 
	);
	glEnableVertexAttribArray(2); // attrib number

	glBindVertexArray(0);			//unbinding
}

void ColorCube::setShader(ShaderProgram *shader)
{
	_shader = shader;
}

void ColorCube::draw()
{
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture.getIdNumber(0));
	glUniform1i(_shader->uniform("ColorTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _texture.getIdNumber(1));
	glUniform1i(_shader->uniform("Tex2"), 1);


	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindTexture(GL_TEXTURE_2D, 0);
}