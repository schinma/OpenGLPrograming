#include "MyGlWindow.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

static float DEFAULT_VIEW_POINT[3] = { -10, 10, 10 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 10, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

MyGlWindow::MyGlWindow(float w, float h)
{
	m_width = w;
	m_height = h;

	setupBuffer();

	m_color_cube = new ColorCube();
	m_floor = new CheckeredFloor(50, 16);
	m_sphere = new Sphere(2, 100, 100);
	m_teapot = new VBOTeapot(8, glm::mat4(1.0));
	m_teapot2 = new VBOTeapot(8, glm::mat4(1.0));
	m_buddha = new Mesh("buddha.obj", m_shaderProgramCube);
	m_dragon = new Mesh("dragon.obj", m_shaderProgramCube);
	m_bunny = new Mesh("bunny.obj", m_shaderProgramCube);
	m_plane = new Plane(100, 20);

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
	delete(m_buddha);
	delete(m_bunny);
	delete(m_dragon);
	delete(m_plane);
	delete(m_shaderProgramCube);
	delete(m_shaderProgramFloor);
	delete(m_viewer);
}

void MyGlWindow::resize(int width, int height) {
	m_height = height;
	m_width = width;
}

void MyGlWindow::setupBuffer()
{
	m_shaderProgramCube = new ShaderProgram();
	m_shaderProgramCube->initFromFiles("fog.vert", "fog.frag");

	m_shaderProgramSil = new ShaderProgram();
	m_shaderProgramSil->initFromFiles("silhouette.vert", "silhouette.frag");	

	m_shaderProgramFloor = new ShaderProgram();
	m_shaderProgramFloor->initFromFiles("simple.vert", "simple.frag");
	
	//add uniform variables
	m_shaderProgramFloor->addUniform("mvp");

	m_shaderProgramCube->addUniform("mvp");
	m_shaderProgramCube->addUniform("Kd"); //Diffuse Object Color :vec3
	m_shaderProgramCube->addUniform("Ka"); //Ambient Object Color : vec3
	m_shaderProgramCube->addUniform("Ks"); //Specular Object Color : vec3
	m_shaderProgramCube->addUniform("shininess");
	m_shaderProgramCube->addUniform("ModelViewMatrix"); //View*Model : mat4
	m_shaderProgramCube->addUniform("NormalMatrix"); //Refer next slide : mat3
	
	m_shaderProgramCube->addUniform("resolution");
	m_shaderProgramCube->addUniform("nbLight");

	for (int i = 0; i < 5; i++) {
		std::string name, name1, name2, name3, name4, name5, name6;
		name = "Light[";
		name1 = name + std::to_string(i) + "].Position";
		name2 = name + std::to_string(i) + "].Intensity";
		name3 = name + std::to_string(i) + "].Direction";
		name4 = name + std::to_string(i) + "].cutOff";
		name5 = name + std::to_string(i) + "].innerCutOff";
		name6 = name + std::to_string(i) + "].exponent";
		m_shaderProgramCube->addUniform(name1);
		m_shaderProgramCube->addUniform(name2);
		m_shaderProgramCube->addUniform(name3);
		m_shaderProgramCube->addUniform(name4);
		m_shaderProgramCube->addUniform(name5);
		m_shaderProgramCube->addUniform(name6);
	}

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

	m_shaderProgramCube->use();
	
	// set the "material" parameter
	glm::vec3 Ka(0.1, 0.1, 0.1);
	glm::vec3 Kd(0.4, 0.4, 0.4);
	glm::vec3 Ks(0.9, 0.9, 0.9);
	GLfloat shiness = 180.0f;
	glUniform3fv(m_shaderProgramCube->uniform("Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(m_shaderProgramCube->uniform("Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(m_shaderProgramCube->uniform("Ks"), 1, glm::value_ptr(Ks));
	glUniform1f(m_shaderProgramCube->uniform("shininess"), shiness);

	glUniform1f(m_shaderProgramCube->uniform("resolution"), (GLfloat)m_width);
	
	// set the lights parameters
	glUniform3fv(m_shaderProgramCube->uniform("Light[0].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.8f, 0.8f)));
	glUniform3fv(m_shaderProgramCube->uniform("Light[1].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.8f)));
	glUniform3fv(m_shaderProgramCube->uniform("Light[2].Intensity"), 1, glm::value_ptr(glm::vec3(0.8f, 0.0f, 0.0f)));
	glUniform3fv(m_shaderProgramCube->uniform("Light[3].Intensity"), 1, glm::value_ptr(glm::vec3(0.0f, 0.8f, 0.0f)));
	glUniform3fv(m_shaderProgramCube->uniform("Light[4].Intensity"), 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));

	glUniform1d(m_shaderProgramCube->uniform("nbLight"), param.LightNumber);
	
	glm::vec4 lightPos(param.X_translation, param.Y_translation, param.Z_translation, 1);

	glm::vec4 viewPos = view * lightPos;
	glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;

	for (int i = 0, theta = param.Rotation; i < 5; i++, theta+=72) {
		float x = param.Radius * cos(theta*(glm::pi<float>() / 180)) + lightPos.x;
		float z = param.Radius * sin(theta*(glm::pi<float>() / 180)) + lightPos.z;
		glm::vec4 newPos = vec4(x, lightPos.y, z, 1);
		glm::vec4 viewPos = view * newPos;
		glm::vec4 direction = view * glm::vec4(0, 0, 0, 1) - viewPos;
		glUniform4fv(m_shaderProgramCube->uniform("Light[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(viewPos));
		glUniform4fv(m_shaderProgramCube->uniform("Light[" + std::to_string(i) + "].Direction"), 1, glm::value_ptr(direction));
		glUniform1f(m_shaderProgramCube->uniform("Light[" + std::to_string(i) + "].cutOff"), 40.0f);
		glUniform1f(m_shaderProgramCube->uniform("Light[" + std::to_string(i) + "].innerCutOff"), 30.0f);
		glUniform1f(m_shaderProgramCube->uniform("Light[" + std::to_string(i) + "].exponent"), 10.0f);
	}

	// Draw the Color Cube
	//draw_object(m_color_cube, projection, view, glm::vec3(0, 1, 0), glm::vec4(0, 0, 0, 0));

	// draw the sphere 
	//draw_object(m_sphere, projection, view, glm::vec3(0, 3, 5), glm::vec4(0, 0, 0, 0));

	// draw the teapot	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	draw_object(m_teapot, projection, view, glm::vec3(-2, 0, -5), glm::vec4(-90, 1, 0, 0));
	draw_object(m_teapot, projection, view, glm::vec3(5, 0, 5), glm::vec4(-90, 1, 0, 0));


	glDisable(GL_CULL_FACE);

	// draw the buddha		
	//draw_object(m_buddha, projection, view, glm::vec3(10, 4, 10), glm::vec4(45, 0, 1, 0), glm::vec3(10, 10, 10));

	//draw the floor
	draw_object(m_plane, projection, view, glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 0));

	// draw the dragon	
	//draw_object(m_dragon, projection, view, glm::vec3(0, 3, 0), glm::vec4(0, 0, 0, 0), glm::vec3(10, 10, 10));

	// draw the bunny
	//draw_object(m_bunny, projection, view, glm::vec3(10, 0, -10), glm::vec4(0, 0, 0, 0), glm::vec3(5, 5, 5));
	
	m_shaderProgramCube->disable();

	m_shaderProgramSil->use();

	glUniform1f(m_shaderProgramSil->uniform("u_offset"), 0.05f);
	glUniform3fv(m_shaderProgramSil->uniform("u_color"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	draw_object(m_teapot2, projection, view, glm::vec3(-2, 0, -5), glm::vec4(-90, 1, 0, 0));

	glDisable(GL_CULL_FACE);

	m_shaderProgramSil->disable();
}

void MyGlWindow::draw_object(IObject *object, glm::mat4 projection, glm::mat4 view, glm::vec3 translation, glm::vec4 rotation, glm::vec3 scale)
{
	glm::mat4 model;
	glm::mat4 modelView;
	glm::mat4 inverseModelView;
	glm::mat3 normalMatrix;
	glm::mat4 mvp;

	m_model.glPushMatrix();

	m_model.glTranslate(translation.x, translation.y, translation.z);
	m_model.glScale(scale.x, scale.y, scale.z);
	if (rotation.x != 0) {
		m_model.glRotate(rotation.x, rotation.y, rotation.z, rotation.w);
	}

	model = m_model.getMatrix();
	modelView = view * model;
	inverseModelView = glm::inverse(modelView);
	normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	mvp = projection * view * model;

	glUniformMatrix4fv(m_shaderProgramCube->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(m_shaderProgramCube->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(m_shaderProgramCube->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
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