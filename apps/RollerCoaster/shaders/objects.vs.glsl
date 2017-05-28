#version 330

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix; 
uniform mat4 uNormalMatrix;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;

out vec2 vTextCoords;
out vec3 vViewSpacePosition;
out vec3 vViewSpaceNormal;

void main()
{
	vViewSpacePosition = (uModelViewMatrix * vec4(aPosition,1)).xyz;
	vViewSpaceNormal = (uNormalMatrix * vec4(aNormal,0)).xyz;

	vTextCoords = aTextCoords;
	gl_Position = uModelViewProjMatrix * vec4(aPosition,1);
}