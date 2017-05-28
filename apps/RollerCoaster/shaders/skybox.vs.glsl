#version 330 core

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

layout(location = 0) in vec3 aPosition;

out vec3 vTextCoords;


void main()
{
	gl_Position = uModelViewProjMatrix * vec4(aPosition, 1);
	vTextCoords = aPosition;
}