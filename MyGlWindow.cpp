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

	m_TextureShader = new TextureShader();
	//m_SkyboxShader = new RefractShader("cube_map_refraction");

	//add uniform variables

	m_TextureShader->addUniforms();
	//m_PhongShader->addUniforms();
	//m_NormalMapShader->addUniforms();
	//m_SkyboxShader->addUniforms();

	m_mesh = new Mesh("mount.blend1.obj", m_TextureShader->getShaderProgram());

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	setupBuffer();
}

MyGlWindow::~MyGlWindow()
{
	delete(m_viewer);
	delete(m_mesh);
	//delete(m_NormalMapShader);
	//delete(m_SkyboxShader);
}

void MyGlWindow::resize(int width, int height) {
	m_height = height;
	m_width = width;
}

void MyGlWindow::setupBuffer()
{

	ctv = new TextureViewer(0, "shaders/textureViewer.vert", "shaders/textureViewer.frag");

	texManager.createTexture("render_tex", "", 800, 800, GL_NEAREST, GL_RGB, GL_RGB, false);
	texManager.createTexture("depth_tex", "", 800, 800, GL_LINEAR, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, true);
	
	fbo = new FboManager();

	fbo->initFbo();
	fbo->bindToFbo(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texManager["render_tex"]);
	fbo->bindToFbo(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texManager["depth_tex"]);
	fbo->setDrawBuffers();

	if (!fbo->checkFboStatus()) {
		std::cout << "error with fbo" << std::endl;
		return;
	}
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

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/


	draw_object(param, m_mesh, m_TextureShader, projection, view, glm::vec3(0, 0, 0), std::vector<glm::vec4>{glm::vec4(0, 0, 0, 0)}, glm::vec3(1, 1, 1));

}

void MyGlWindow::draw_object(Parameter param ,IObject *object, Shader * shader, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, std::vector<glm::vec4> rotations, glm::vec3 scale)
{
	glm::mat4 model;
	glm::mat4 modelView;
	glm::mat4 inverseModelView;
	glm::mat3 normalMatrix;
	glm::mat4 mvp;
	glm::vec3 matColor = glm::vec3(0.4, 0.4, 0.4);

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

	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->getFboId());
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	shader->getShaderProgram()->use();
	shader->setUniforms(view, param);

	glUniformMatrix4fv(shader->getShaderProgram()->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(shader->getShaderProgram()->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shader->getShaderProgram()->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	shader->getShaderProgram()->disable();

	object->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	ctv->setTexture(texManager["render_tex"]);
	ctv->draw();

	m_model.glPopMatrix();

}

void MyGlWindow::draw_floor(glm::mat4 projection, glm::mat4 view)
{

}