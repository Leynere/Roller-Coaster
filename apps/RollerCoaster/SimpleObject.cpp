#include "SimpleObject.hpp"

#include <iostream>

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void SimpleObject::draw(GLint uModelViewProjMatrixID, GLint uModelViewMatrixID, GLint uNormalMatrixID,glm::mat4 projection, glm::mat4 view, glm::mat4 model){

    glBindVertexArray(vao);

        glm::mat4 modelView = view * model;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelView));
        glm::mat4 modelViewProj = projection * view * model;

        glUniformMatrix4fv(uModelViewProjMatrixID,1,GL_FALSE,glm::value_ptr(modelViewProj));
        glUniformMatrix4fv(uModelViewMatrixID,1,GL_FALSE,glm::value_ptr(modelView));
        glUniformMatrix4fv(uNormalMatrixID,1,GL_FALSE,glm::value_ptr(normalMatrix));
        
        glDrawElements(GL_TRIANGLES, obj->indexBuffer.size(), GL_UNSIGNED_INT, nullptr);    

    glBindVertexArray(0);
}

SimpleObject::SimpleObject(glmlv::SimpleGeometry* objectToCreate, GLuint m_program){

    const GLint positionAttrLocation = glGetAttribLocation(m_program, "aPosition");
    const GLint normalAttrLocation = glGetAttribLocation(m_program, "aNormal");
    const GLint textCoordsAttrLocation = glGetAttribLocation(m_program, "aTextCoords");

   	obj = objectToCreate;

	glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

        glBufferStorage(GL_ARRAY_BUFFER,obj->vertexBuffer.size() * sizeof(glmlv::Vertex3f3f2f), obj->vertexBuffer.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER,0);


    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ARRAY_BUFFER,ibo);

        glBufferStorage(GL_ARRAY_BUFFER,obj->indexBuffer.size() * sizeof(uint32_t),obj->indexBuffer.data(),0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER,vbo);

            if(positionAttrLocation != -1){
                glEnableVertexAttribArray(positionAttrLocation);
                glVertexAttribPointer(positionAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));
            }

            if(normalAttrLocation != -1){
                glEnableVertexAttribArray(normalAttrLocation);
                glVertexAttribPointer(normalAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));
            }
            if(textCoordsAttrLocation != -1){
                glEnableVertexAttribArray(textCoordsAttrLocation);
                glVertexAttribPointer(textCoordsAttrLocation, 2, GL_FLOAT, GL_FALSE, sizeof(glmlv::Vertex3f3f2f), (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));
            }
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);
}

SimpleObject::~SimpleObject(){
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }

    if (ibo) {
        glDeleteBuffers(1, &ibo);
    }

    if (vao) {
        glDeleteBuffers(1, &vao);
    }
}
SimpleObject::SimpleObject(){

}