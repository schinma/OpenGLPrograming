

#define GLM_ENABLE_EXPERIMENTAL 

#ifndef CTEXTUREVIEWER_H
#define CTEXTUREVIEWER_H
#include <iostream>
#include <map>
#include <vector>


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Loader.h"


/*
* Simple class, which will render texture on screen 
*/
class TextureViewer
{
private:

	GLuint texture;
	//VBO - don't need EBO, i'll use glDrawArrays()
	GLuint VBO;
	//VAO - needed for glDrawArrays()
	GLuint VAO;
	ShaderProgram * s;
	//Default shaders
	std::string vs;
	std::string fs;
	bool depth;
	void setUpShaders();
	

public:

	TextureViewer();
	TextureViewer(GLuint tex, std::string vs, std::string fs);
	void draw();
	//Setters
	void setTexture(GLuint tex);
	void setDepthOnly(bool depth);
	//Getters
	GLuint getTexture();
	~TextureViewer();
};

#endif

