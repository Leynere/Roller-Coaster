#version 330

in vec2 vTextCoords;
in vec3 vViewSpacePosition;
in vec3 vViewSpaceNormal;

out vec3 fColor;
uniform sampler2D uSampler;

uniform vec3 uDirectionalLightDir;
uniform vec3 uDirectionalLightIntensity;

uniform vec3 uPointLightPosition;
uniform vec3 uPointLightIntensity;


void main()
{
	fColor = vec3(texture(uSampler, vTextCoords));
	float distToPointLight = length(uPointLightPosition - vViewSpacePosition);
	vec3 dirToPointLight = (uPointLightPosition - vViewSpacePosition) / distToPointLight;
	vec3 kd = vec3(texture(uSampler, vTextCoords));
	fColor = kd * (uDirectionalLightIntensity * max(0.0, dot(vViewSpaceNormal, uDirectionalLightDir))
		+ uPointLightIntensity * max(0.0, dot(vViewSpaceNormal, dirToPointLight)) / (distToPointLight * distToPointLight));
}