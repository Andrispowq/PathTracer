#version 430

layout (location = 0) out vec4 outColour;

layout (location = 0) in vec3 position_FS;

uniform samplerCube environmentMap;
uniform float lod;

uniform float exposure;
uniform float gamma;

void main()
{
	vec3 envColour = textureLod(environmentMap, normalize(position_FS), lod).rgb;

	envColour = 1.0 - exp(-envColour * exposure);
	envColour = pow(envColour, vec3(1.0 / gamma));

	outColour = vec4(envColour, 0.0);
}