#pragma once

#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

#include <iostream>
#include "GL/glew.h"
#include <string>
#include "Loader.h"
#include "ColorCube.h"
#include "ModelView.h"
#include "Viewer.h"
#include "glm/gtc/type_ptr.hpp"
#include "CheckeredFloor.h"
#include "WireCube.h"
#include "RedLine.h"
#include "Sphere.h"
#include "vboteapot.h"
#include "Mesh.h"
#include "IObject.h"
#include "Plane.h"
#include "Material.h"
#include "TextureShader.h"
#include "PhongShader.h"

class MyGlWindow {
public:
	MyGlWindow(float w, float h);
	~MyGlWindow();
	void draw(Parameter param);
	void resize(int width, int height);

	Viewer *m_viewer;

private:

	//Shaders
	TextureShader *m_TextureShader;
	PhongShader *m_PhongShader;
	
	ShaderProgram *m_shaderProgramSil;
	ShaderProgram *m_shaderProgramFloor;
	
	float m_width;
	float m_height;
	
	//Objects
	IObject *m_wire_cube;
	IObject *m_color_cube;
	RedLine *m_line;
	IObject *m_sphere;
	IObject *m_teapot;
	IObject *m_teapot2;
	CheckeredFloor *m_floor;
	IObject *m_buddha;
	IObject *m_dragon;
	IObject *m_bunny;
	IObject *m_plane;

	Model m_model;

	void setupBuffer();
	void draw_floor(glm::mat4 proj, glm::mat4 view);
	void draw_object(IObject *objectn, Shader *shader, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, 
		std::vector<glm::vec4> rotations, glm::vec3 scale = glm::vec3(1, 1, 1));
};

#endif