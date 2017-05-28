#pragma once

#include <glmlv/filesystem.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/simple_geometry.hpp>
#include "SimpleObject.hpp"
#include <glmlv/ViewController.hpp>

class Application
{
public:
	Application(int argc, char** argv);
	~Application();

	int run();
private:
	const int m_nWindowWidth = 1280;
	const int m_nWindowHeight = 720;
	glmlv::GLFWHandle m_GLFWHandle{ m_nWindowWidth, m_nWindowHeight, "Roller Coaster" }; // Note: the handle must be declared before the creation of any object managing OpenGL resource (e.g. GLProgram, GLShader)

	const glmlv::fs::path m_AppPath;
	const std::string m_AppName;
	const std::string m_ImGuiIniFilename;
	const glmlv::fs::path m_ShadersRootPath;
	const glmlv::fs::path m_AssetsRootPath;
	glmlv::GLProgram m_program;
	glmlv::GLProgram m_programSkybox;

	SimpleObject cube;
	SimpleObject cylinder;
	SimpleObject skyBox;
	SimpleObject plane;

	glmlv::SimpleGeometry simpleSkybox;
	glmlv::SimpleGeometry simpleCylinder;
	glmlv::SimpleGeometry simpleCube;
	glmlv::SimpleGeometry simplePlane;

	GLint uModelViewProjMatrixID = 0;
	GLint uModelViewMatrixID = 0;
	GLint uNormalMatrixID = 0;
	
	GLint uSamplerID = 0;

	GLint uDirectionalLightDirID = 0;
	GLint uDirectionalLightIntensityID = 0;
	GLint uPointLightPositionID = 0;
	GLint uPointLightIntensityID = 0;

	GLint uSkyboxModelViewProjMatrixID = 0;
	GLint uSkyboxModelViewMatrixID = 0;
	GLint uSkyboxNormalMatrixID = 0;
	GLint uSkyboxCubemapID = 0;

	glm::vec3 lightColor = glm::vec3(1, 1, 1);

	glm::vec3 directionalLightDir = glm::vec3(-0.5,1,0);
	float directionalLightIntensity = 0.1f;

	glm::vec3 pointLightPosition = glm::vec3(0,0,0);
	float pointLightIntensity = 0.0f;

	glm::mat4 projection;
	glm::mat4 view;

	std::vector <std::string> texturesNames = { "bois.jpg","metal.jpg","grass.jpg","rocaille.jpg" };
	std::vector<GLuint> texturesIndex;
	GLuint sampler = 0;

	std::vector <std::string> cubecmapsNames = { "ely_hills","hw_nightsky" };
	std::vector<GLuint> cubemapIndex;

	int currentSkybox = 0;
	int currentGround = 2;

	bool isFirstPerson = true;

	void drawARail(glm::mat4 stack);

	void drawChariot(glm::mat4 stack);

	//angle wideness must be 24 at least and multiple of 24.
	void drawPath(float angleWideness = 24);

	float drawAnimatedChariot(glm::mat4* stack, float traveledDistance, float speed = 0.01f, float angleWideness = 24.0f);

	void drawGround(glm::mat4 stack, float nbTiles);

	void drawSkybox();

	std::vector<float> railSteps = { 100,10,100};


	glmlv::ViewController m_viewController{ m_GLFWHandle.window(), 3.f };

};

glmlv::SimpleGeometry makePlane();
glmlv::SimpleGeometry makeCylinder(int subdivByCircle);
