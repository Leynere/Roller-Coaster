#pragma once
#include <glmlv/simple_geometry.hpp>

#include <glmlv/GLProgram.hpp>
#include <glmlv/imgui_impl_glfw_gl3.hpp>


class SimpleObject{
public:
	SimpleObject();
    SimpleObject(glmlv::SimpleGeometry* objectToCreate, GLuint m_program);
    ~SimpleObject();
	void draw(GLint uModelViewProjMatrixID, GLint uModelViewMatrixID, GLint uNormalMatrixID,glm::mat4 projection, glm::mat4 view, glm::mat4 model);    

private:
    glmlv::SimpleGeometry* obj = nullptr;

    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vao = 0;
};