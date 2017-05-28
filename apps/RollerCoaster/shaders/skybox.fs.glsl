#version 330

in vec3 vTextCoords;

uniform samplerCube uCubemap;  

out vec4 fColor;

void main()
{
	fColor = texture(uCubemap, vTextCoords);
}