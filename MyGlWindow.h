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
#include "NormalMapShader.h"
#include "ShaderSkyBox.h"
#include "SkyBox.h"
#include "RefractShader.h"
#include "textureManager.h"
#include "TextureViewer.h"
#include "fboManager.h"


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
	NormalMapShader *m_NormalMapShader;
	RefractShader *m_SkyboxShader;

	FboManager *fbo;
	TextureViewer *ctv;
	TextureManager texManager;
	GLuint fbo_depthId;


	float m_width;
	float m_height;
	
	//Objects
	IObject *m_skybox;
	IObject *m_mesh;
	IObject *m_cube;

	Model m_model;

	void setupBuffer();
	void draw_floor(glm::mat4 proj, glm::mat4 view);
	void draw_object(Parameter param, IObject *objectn, Shader *shader, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, 
		std::vector<glm::vec4> rotations, glm::vec3 scale = glm::vec3(1, 1, 1));
};

#endif