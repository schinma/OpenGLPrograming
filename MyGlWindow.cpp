#include "MyGlWindow.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

static float DEFAULT_VIEW_POINT[3] = { -10, 10, 10 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 10, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

float rotAngle = 0;

MyGlWindow::MyGlWindow(float w, float h)
{
	m_width = w;
	m_height = h;

	setupBuffer();

	m_color_cube = new ColorCube(m_TextureShader->getShaderProgram());
	m_floor = new CheckeredFloor(50, 16);
	m_sphere = new Sphere(2, 100, 100, m_TextureShader->getShaderProgram());
	m_teapot = new VBOTeapot(8, glm::mat4(1.0));
	m_teapot2 = new VBOTeapot(8, glm::mat4(1.0));
	m_plane = new Plane(100, 20);
	m_moutain = new Mesh("mount.blend1.obj", m_TextureShader->getShaderProgram());
	m_ogre = new Mesh("bs_ears.obj", m_NormalMapShader->getShaderProgram());

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
}

MyGlWindow::~MyGlWindow()
{
	delete(m_floor);
	delete(m_color_cube);
	delete(m_sphere);
	delete(m_teapot);
	delete(m_plane);
	delete(m_TextureShader);
	delete(m_PhongShader);
	delete(m_shaderProgramFloor);
	delete(m_shaderProgramSil);
	delete(m_shaderProgramFloor);
	delete(m_viewer);
	delete(m_NormalMapShader);
	delete(m_ogre);
}

void MyGlWindow::resize(int width, int height) {
	m_height = height;
	m_width = width;
}

void MyGlWindow::setupBuffer()
{
	m_TextureShader = new TextureShader();
	m_PhongShader = new PhongShader();
	m_NormalMapShader = new NormalMapShader();

	m_shaderProgramSil = new ShaderProgram();
	m_shaderProgramSil->initFromFiles("shaders/silhouette.vert", "shaders/silhouette.frag");	

	m_shaderProgramFloor = new ShaderProgram();
	m_shaderProgramFloor->initFromFiles("shaders/simple.vert", "shaders/simple.frag");
	
	//add uniform variables
	
	m_TextureShader->addUniforms();
	m_PhongShader->addUniforms();
	m_NormalMapShader->addUniforms();

	m_shaderProgramSil->addUniform("mvp");
	m_shaderProgramSil->addUniform("u_offset");
	m_shaderProgramSil->addUniform("u_color");
}

void MyGlWindow::draw(Parameter param)
{
	glm::vec3 eye;
	glm::vec3 look;
	glm::vec3 up = m_viewer->getUpVector();

	eye = m_viewer->getViewPoint();
	look = m_viewer->getViewCenter();

	glm::mat4 view = lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, m_width / m_height, 0.1f, 500.0f);

	//draw_floor(projection, view);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Texture Shader
	m_TextureShader->getShaderProgram()->use();
	m_TextureShader->setUniforms(view, param);
	
	draw_object(m_color_cube, m_TextureShader, projection, view, glm::vec3(0, 1, 0), std::vector<glm::vec4>{glm::vec4(-90, 1, 0, 0)});

	//rotAngle++;
	draw_object(m_sphere, m_TextureShader, projection, view, glm::vec3(0, 3, 5), std::vector<glm::vec4>{glm::vec4(270, 1, 0, 0), glm::vec4(rotAngle, 0, 0, 1)});

	draw_object(m_moutain, m_TextureShader, projection, view, glm::vec3(10, 4, 10), std::vector<glm::vec4>{glm::vec4(45, 0, 1, 0)});

	m_TextureShader->getShaderProgram()->disable();

	//Normal Map Shader
	m_NormalMapShader->getShaderProgram()->use();
	m_NormalMapShader->setUniforms(view, param);

	draw_object(m_ogre, m_NormalMapShader, projection, view, glm::vec3(-10, 4, -10), std::vector<glm::vec4>{}, glm::vec3(5, 5, 5));
	
	m_NormalMapShader->getShaderProgram()->disable();
	
	m_PhongShader->getShaderProgram()->use();	
	m_PhongShader->setUniforms(view, param);
	//draw the floor
	draw_object(m_plane, m_PhongShader, projection, view, glm::vec3(0, 0, 0), std::vector<glm::vec4>{glm::vec4(0, 0, 0, 0)});
	
	m_PhongShader->getShaderProgram()->disable();

}

void MyGlWindow::draw_object(IObject *object, Shader * shader, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, std::vector<glm::vec4> rotations, glm::vec3 scale)
{
	glm::mat4 model;
	glm::mat4 modelView;
	glm::mat4 inverseModelView;
	glm::mat3 normalMatrix;
	glm::mat4 mvp;

	m_model.glPushMatrix();

	m_model.glTranslate(translation.x, translation.y, translation.z);
	m_model.glScale(scale.x, scale.y, scale.z);
		
	for (auto &rotation : rotations) {
		if (rotation.x != 0) 
			m_model.glRotate(rotation.x, rotation.y, rotation.z, rotation.w);
	}

	model = m_model.getMatrix();
	modelView = view * model;
	inverseModelView = glm::inverse(modelView);
	normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	mvp = projection * view * model;

	glUniformMatrix4fv(shader->getShaderProgram()->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(shader->getShaderProgram()->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shader->getShaderProgram()->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	
	glUniformMatrix4fv(m_shaderProgramSil->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	object->draw();
	m_model.glPopMatrix();
}

void MyGlWindow::draw_floor(glm::mat4 projection, glm::mat4 view)
{
	glm::mat4 mvp;
	glm::mat4 model;

	m_shaderProgramFloor->use();
	
	m_model.glPushMatrix(); 
	m_model.glTranslate(0, 0, 0);
	model = m_model.getMatrix();
	mvp = projection * view * model;

	glUniformMatrix4fv(m_shaderProgramFloor->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	m_floor->draw();
	m_model.glPopMatrix();

	m_shaderProgramFloor->disable();
}