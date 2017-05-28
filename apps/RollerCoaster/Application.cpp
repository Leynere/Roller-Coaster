#include "Application.hpp"

#include <iostream>

#include <glmlv/Image2DRGBA.hpp>
#include <imgui.h>
#include <glmlv/imgui_impl_glfw_gl3.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

glmlv::SimpleGeometry makePlane() {
	std::vector<glmlv::Vertex3f3f2f> vertexBuffer = {
		{ glm::vec3(-0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) },
		{ glm::vec3(0.5, 0, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
		{ glm::vec3(-0.5, 0, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) }
	};


	std::vector<uint32_t> indexBuffer = {
		0, 1, 2,
		0, 2, 3
	};

	return{ vertexBuffer, indexBuffer };
}

glmlv::SimpleGeometry makeCylinder(int subdivByCircle = 6) {
	
	float iter = (2 * glm::pi<float>()) / subdivByCircle;
	std::vector<glmlv::Vertex3f3f2f> vertexBuffer;

	//left side
	vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(-0.5, 0, 0), glm::vec3(-1, 0, 0), glm::vec2(0.5,0)));
	for (int i = 0; i < subdivByCircle; i++) {
		vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(-0.5,glm::cos(i*iter),glm::sin(i*iter)), glm::vec3(-1,0,0), glm::vec2(0,1) ));
	}
	int offsetRightSide = vertexBuffer.size();

	//right size
	vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(0.5, 0, 0), glm::vec3(1, 0, 0), glm::vec2(0.5, 0)));
	for (int i = 0; i < subdivByCircle; i++) {
		vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(0.5, glm::cos(i*iter), glm::sin(i*iter)), glm::vec3(1, 0, 0), glm::vec2(0, 1)));
	}
	int offsetBetweenTheTwoCircle = vertexBuffer.size();

	//betweenTheTwoCircles
	for (int i = 0; i < subdivByCircle; i++) {
		vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(0.5, glm::cos(i*iter), glm::sin(i*iter)), glm::vec3(0, glm::cos(i*iter), glm::sin(i*iter)), glm::vec2(0, 1)));
		vertexBuffer.emplace_back(glmlv::Vertex3f3f2f(glm::vec3(-0.5, glm::cos(i*iter), glm::sin(i*iter)), glm::vec3(0, glm::cos(i*iter), glm::sin(i*iter)), glm::vec2(1, 1)));
	}

	std::vector<uint32_t> indexBuffer;
	//left side
	for (int i = 1; i < subdivByCircle - 1; i++) {
		indexBuffer.emplace_back(0);
		indexBuffer.emplace_back(i);
		indexBuffer.emplace_back(i + 1);
	}

	indexBuffer.emplace_back(0);
	indexBuffer.emplace_back(subdivByCircle - 1);
	indexBuffer.emplace_back(1);

	//right side
	for (int i = 1; i < subdivByCircle - 1; i++) {
		indexBuffer.emplace_back(offsetRightSide + 0);
		indexBuffer.emplace_back(offsetRightSide + i);
		indexBuffer.emplace_back(offsetRightSide + i + 1);
	}

	indexBuffer.emplace_back(offsetRightSide + 0);
	indexBuffer.emplace_back(offsetRightSide + subdivByCircle - 1);
	indexBuffer.emplace_back(offsetRightSide + 1);

	//betweenTheTwoCircles
	for (int i = 0; i < subdivByCircle*2 -2; i++) {
		indexBuffer.emplace_back(offsetBetweenTheTwoCircle + i);
		indexBuffer.emplace_back(offsetBetweenTheTwoCircle + i + 1);
		indexBuffer.emplace_back(offsetBetweenTheTwoCircle + i + 2);
	}

	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + 0);
	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + subdivByCircle*2 - 1);
	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + subdivByCircle*2 - 2);
	
	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + 1);
	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + subdivByCircle*2 - 1);
	indexBuffer.emplace_back(offsetBetweenTheTwoCircle + 0);


	return{ vertexBuffer, indexBuffer };

}

void Application::drawChariot(glm::mat4 stack) {
	float valueToTranslate = 0.45;

	glBindTexture(GL_TEXTURE_2D, texturesIndex[1]);
	//left side
	glm::mat4 cubeModel = glm::translate(glm::mat4(), glm::vec3(valueToTranslate, 0, 0));
	cubeModel = glm::scale(cubeModel, glm::vec3(0.1, 1, 1.8));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cubeModel);

	//right side
	cubeModel = glm::translate(glm::mat4(), glm::vec3(-valueToTranslate, 0, 0));
	cubeModel = glm::scale(cubeModel, glm::vec3(0.1, 1, 1.8));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cubeModel);

	//top
	cubeModel = glm::translate(glm::mat4(), glm::vec3(0, 0, valueToTranslate * 2));
	cubeModel = glm::rotate(cubeModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
	cubeModel = glm::scale(cubeModel, glm::vec3(0.1, 1, 1));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cubeModel);

	//down
	cubeModel = glm::translate(glm::mat4(), glm::vec3(0, 0, -valueToTranslate * 2));
	cubeModel = glm::rotate(cubeModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
	cubeModel = glm::scale(cubeModel, glm::vec3(0.1, 1, 1));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cubeModel);

	//bottom
	cubeModel = glm::translate(glm::mat4(), glm::vec3(0, -valueToTranslate, 0));
	cubeModel = glm::scale(cubeModel, glm::vec3(1, 0.1, 1.8));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cubeModel);

	glBindTexture(GL_TEXTURE_2D, texturesIndex[0]);
	//left down tire
	glm::mat4 cylinderModel = glm::translate(glm::mat4(), glm::vec3(valueToTranslate, -0.5, -valueToTranslate * 2));
	cylinderModel = glm::translate(cylinderModel, glm::vec3(0.3,0,0));
	cylinderModel = glm::scale(cylinderModel, glm::vec3(0.5, 0.5, 0.5));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cylinderModel);

	//right down tire
	cylinderModel = glm::translate(glm::mat4(), glm::vec3(-valueToTranslate, -0.5, -valueToTranslate * 2));
	cylinderModel = glm::translate(cylinderModel, glm::vec3(-0.3, 0, 0));
	cylinderModel = glm::scale(cylinderModel, glm::vec3(0.5, 0.5, 0.5));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cylinderModel);

	//left top tire
	cylinderModel = glm::translate(glm::mat4(), glm::vec3(valueToTranslate, -0.5, valueToTranslate * 2));
	cylinderModel = glm::translate(cylinderModel, glm::vec3(0.3, 0, 0));
	cylinderModel = glm::scale(cylinderModel, glm::vec3(0.5, 0.5, 0.5));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cylinderModel);

	//right top tire
	cylinderModel = glm::translate(glm::mat4(), glm::vec3(-valueToTranslate, -0.5, valueToTranslate * 2));
	cylinderModel = glm::translate(cylinderModel, glm::vec3(-0.3, 0, 0));
	cylinderModel = glm::scale(cylinderModel, glm::vec3(0.5, 0.5, 0.5));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * cylinderModel);
}

void Application::drawARail(glm::mat4 stack) {
	float valueToTranslate = 0.8;
	glBindTexture(GL_TEXTURE_2D, texturesIndex[1]);

	//left rail
	glm::mat4 railModel = glm::translate(glm::mat4(), glm::vec3(-valueToTranslate, valueToTranslate/2, 0));
	railModel = glm::rotate(railModel, glm::pi<float>() / 2, glm::vec3(0,1,0));
	railModel = glm::scale(railModel, glm::vec3(2, 0.1, 0.1));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * railModel);

	//right rail
	railModel = glm::translate(glm::mat4(), glm::vec3(valueToTranslate, valueToTranslate/2, 0));
	railModel = glm::rotate(railModel, glm::pi<float>() / 2, glm::vec3(0,1,0));
	railModel = glm::scale(railModel, glm::vec3(2, 0.1, 0.1));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * railModel);


	//center rail
	railModel = glm::mat4();
	railModel = glm::rotate(railModel, glm::pi<float>() / 2, glm::vec3(0,1,0));
	railModel = glm::scale(railModel, glm::vec3(2, 0.1, 0.1));
	cylinder.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * railModel);

	//left join
	glm::mat4 join = glm::translate(glm::mat4(), glm::vec3(-valueToTranslate / 2, valueToTranslate /4, 0));
	join = glm::rotate(join,-glm::pi<float>() / 8, glm::vec3(0,0,1));
	join = glm::scale(join,glm::vec3(1, 0.2, 0.2));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * join);

	//right join
	join = glm::translate(glm::mat4(), glm::vec3(valueToTranslate / 2, valueToTranslate/4, 0));
	join = glm::rotate(join, glm::pi<float>() / 8, glm::vec3(0, 0, 1));
	join = glm::scale(join, glm::vec3(1, 0.2, 0.2));
	cube.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * join);
}


void Application::drawPath(float angleWideness) {
	glm::mat4 stack = glm::mat4();
	float rotationValue = glm::pi<float>() / angleWideness;
	for (int i = 0; i < railSteps[0]; i++) {
		stack = glm::translate(stack, glm::vec3(glm::sin(rotationValue), 0, 1.8 * glm::cos(rotationValue)));
		stack = glm::rotate(stack, rotationValue, glm::vec3(0, 1, 0));

		stack = glm::rotate(stack, rotationValue / 8, glm::vec3(0, 0, 1));
		drawARail(stack);
	}

	for (int i = 0; i < railSteps[1]; i++) {
		stack = glm::translate(stack, glm::vec3(0, 0, 1));
		drawARail(stack);
	}

	for (int i = 0; i < railSteps[2]; i++) {
		stack = glm::translate(stack, glm::vec3(-glm::sin(rotationValue), 0, 1.8 * glm::cos(rotationValue)));
		stack = glm::rotate(stack, -rotationValue, glm::vec3(0, 1, 0));

		stack = glm::rotate(stack, rotationValue / 8, glm::vec3(0, 0, 1));
		drawARail(stack);
	}
}

float Application::drawAnimatedChariot(glm::mat4 *stack, float traveledDistance, float speed, float angleWideness) {

	float rotationValue = glm::pi<float>() / 24.0f;
	float distanceToTravel = 0.01f * (1000.0f / ImGui::GetIO().Framerate);
	traveledDistance += distanceToTravel;
	if (traveledDistance <= railSteps[0]) {
		*stack = glm::translate(*stack, distanceToTravel * glm::vec3(distanceToTravel * glm::sin(rotationValue), 0, 1.8 * glm::cos(rotationValue)));
		*stack = glm::rotate(*stack, distanceToTravel * rotationValue, glm::vec3(0, 1, 0));

		*stack = glm::rotate(*stack, distanceToTravel * rotationValue / 8, glm::vec3(0, 0, 1));
	}
	else if (traveledDistance <= railSteps[0] + railSteps[1]) {
		*stack = glm::translate(*stack, distanceToTravel * glm::vec3(0, 0, 1));
	}
	else if (traveledDistance <= railSteps[0] + railSteps[1] + railSteps[2]) {
		*stack = glm::translate(*stack, distanceToTravel * glm::vec3(distanceToTravel * -glm::sin(rotationValue), 0, 1.8 * glm::cos(rotationValue)));
		*stack = glm::rotate(*stack, distanceToTravel * -rotationValue, glm::vec3(0, 1, 0));

		*stack = glm::rotate(*stack, distanceToTravel * rotationValue / 8, glm::vec3(0, 0, 1));
	}
	else {
		traveledDistance = 0;
		*stack = glm::translate(glm::mat4(), glm::vec3(0, 1.5, 0));
	}

	if (isFirstPerson == true) {
		view = m_viewController.getViewMatrix() * glm::inverse(*stack);
	}

	pointLightPosition = glm::vec3((*stack)[3][0], (*stack)[3][1], (*stack)[3][2]);
	glUniform3fv(uPointLightPositionID, 1, glm::value_ptr(glm::vec3(view * glm::vec4(pointLightPosition, 1))));
	glUniform3fv(uPointLightIntensityID, 1, glm::value_ptr(lightColor * pointLightIntensity));

	drawChariot(*stack);
	return traveledDistance;
}

void Application::drawGround(glm::mat4 stack,float nbTiles) {

	glBindTexture(GL_TEXTURE_2D, texturesIndex[currentGround]);
	for (int i = 0; i < nbTiles; i++) {
		for (int j = 0; j < nbTiles; j++) {
			glm::mat4 groundMatrix = glm::mat4();
			groundMatrix = glm::translate(groundMatrix, glm::vec3(i/nbTiles,0,j/nbTiles));
			groundMatrix = glm::scale(groundMatrix, glm::vec3(1.0f/nbTiles, 1, 1.0f/nbTiles));
			plane.draw(uModelViewProjMatrixID, uModelViewMatrixID, uNormalMatrixID, projection, view, stack * groundMatrix);
		}
	}
}
void Application::drawSkybox() {

	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapIndex[currentSkybox]);
	float skyboxScale = 1000.0f;
	glm::mat4 skyboxMatrix = glm::mat4();
	skyboxMatrix = glm::translate(skyboxMatrix, glm::vec3(0, 10, 0));
	skyboxMatrix = glm::scale(skyboxMatrix, glm::vec3(skyboxScale, skyboxScale, skyboxScale));
	cube.draw(uSkyboxModelViewProjMatrixID, uSkyboxModelViewMatrixID, uSkyboxNormalMatrixID, projection, view, skyboxMatrix);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthMask(GL_TRUE);
}

int Application::run()
{
    float clearColor[3] = { 0, 0, 0 };
    // Loop until the user closes the window
	glm::mat4 stack = glm::translate(glm::mat4(), glm::vec3(0, 1.5, 0));
	float traveledDistance = 0;
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
		const auto seconds = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Put here rendering code

		m_programSkybox.use();
		drawSkybox();

		m_program.use();

		glUniform3fv(uDirectionalLightDirID, 1, glm::value_ptr(glm::vec3(view *glm::vec4(glm::normalize(directionalLightDir),0))));
		glUniform3fv(uDirectionalLightIntensityID, 1, glm::value_ptr(lightColor * directionalLightIntensity));

		glUniform1i(uSamplerID, 0);
		glBindSampler(0, sampler);


		traveledDistance = drawAnimatedChariot(&stack, traveledDistance);
		float groundScaleValue = 1000.0f;
		glm::mat4 groundMatrix = glm::mat4();
		groundMatrix = glm::translate(groundMatrix, glm::vec3(-groundScaleValue / 2, 0, -groundScaleValue / 2));
		groundMatrix = glm::scale(groundMatrix, glm::vec3(groundScaleValue, 1, groundScaleValue));
		drawGround(groundMatrix, 10);

		drawPath();
		if (!isFirstPerson) {
			view = m_viewController.getViewMatrix();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindSampler(0, 0);


		// GUI code:
        ImGui_ImplGlfwGL3_NewFrame();
		
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::RadioButton("Day", currentSkybox == 0)) {
				currentSkybox = 0;
				currentGround = 2;
				directionalLightIntensity = 0.1f;
				pointLightIntensity = 0.0f;
			}

			if (ImGui::RadioButton("Night", currentSkybox == 1)) {
				currentSkybox = 1;
				currentGround = 3;
				directionalLightIntensity = 0.0f;
				pointLightIntensity = 5.0f;
			}

			if (ImGui::RadioButton("Global view", !isFirstPerson)) {
				isFirstPerson = false;
				m_viewController.setViewMatrix(glm::lookAt(glm::vec3(-10, 20, -15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

			}

			if (ImGui::RadioButton("First person view", isFirstPerson)) {
				isFirstPerson = true;
				m_viewController.setViewMatrix(glm::lookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));

			}

        }

        const auto viewportSize = m_GLFWHandle.framebufferSize();
        glViewport(0, 0, viewportSize.x, viewportSize.y);
        ImGui::Render();

        /* Poll for and process events */
        glfwPollEvents();

        /* Swap front and back buffers*/
        m_GLFWHandle.swapBuffers();

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
			m_viewController.update(float(ellapsedTime));
        }
    }

    return 0;
}

Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" },
	m_AssetsRootPath {	m_AppPath.parent_path() / "assets"}
{
    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file
    m_program = glmlv::compileProgram({ m_ShadersRootPath / m_AppName / "objects.vs.glsl", m_ShadersRootPath / m_AppName / "objects.fs.glsl" });

    uModelViewProjMatrixID = glGetUniformLocation(m_program.glId(),"uModelViewProjMatrix");
    uModelViewMatrixID = glGetUniformLocation(m_program.glId(),"uModelViewMatrix"); 
	uNormalMatrixID = glGetUniformLocation(m_program.glId(), "uNormalMatrix");
	
	uSamplerID = glGetUniformLocation(m_program.glId(),"uSampler");

	uDirectionalLightDirID = glGetUniformLocation(m_program.glId(), "uDirectionalLightDir");
	uDirectionalLightIntensityID = glGetUniformLocation(m_program.glId(), "uDirectionalLightIntensity");
	uPointLightPositionID = glGetUniformLocation(m_program.glId(), "uPointLightPosition");
	uPointLightIntensityID = glGetUniformLocation(m_program.glId(), "uPointLightIntensity");

	m_programSkybox = glmlv::compileProgram({ m_ShadersRootPath / m_AppName / "skybox.vs.glsl", m_ShadersRootPath / m_AppName / "skybox.fs.glsl" });
	
	uSkyboxModelViewProjMatrixID = glGetUniformLocation(m_programSkybox.glId(), "uModelViewProjMatrix");
	uSkyboxModelViewMatrixID = glGetUniformLocation(m_programSkybox.glId(), "uModelViewMatrix");
	uSkyboxNormalMatrixID = glGetUniformLocation(m_programSkybox.glId(), "uNormalMatrix");
	uSkyboxCubemapID = glGetUniformLocation(m_programSkybox.glId(), "uCubemap");

    simpleCube = glmlv::makeCube();
	simpleCylinder = makeCylinder(30);
	simplePlane = makePlane();

	cylinder = SimpleObject(&simpleCylinder, m_program.glId());
    cube = SimpleObject(&simpleCube,m_program.glId());
	plane = SimpleObject(&simplePlane, m_program.glId());

	for (int i = 0; i < texturesNames.size(); i++) {
		texturesIndex.emplace_back(0);
		glGenTextures(1, &(texturesIndex[i]));
		glmlv::Image2DRGBA image = glmlv::readImage({ m_AssetsRootPath / m_AppName / texturesNames[i] });
		glBindTexture(GL_TEXTURE_2D, texturesIndex[i]);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, image.width(), image.height());
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	for (int i = 0; i < 2; i++) {
		cubemapIndex.emplace_back(0);
		glGenTextures(1, &(cubemapIndex[i]));

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapIndex[i]);
			std::vector<std::string> images = { "ft.tga","bk.tga","up.tga","dn.tga","rt.tga","lf.tga" };
			for (int j = 0; j < images.size(); j++) {
				glmlv::Image2DRGBA image = glmlv::readImage({ m_AssetsRootPath / m_AppName / cubecmapsNames[i] / images[j]});
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGBA8, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	m_viewController.setViewMatrix(glm::lookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));

    glEnable(GL_DEPTH_TEST);
}


Application::~Application()
{
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
}
