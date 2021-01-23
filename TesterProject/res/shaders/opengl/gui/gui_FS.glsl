#version 330 

layout (location = 0) out vec4 outColour;

in vec2 texture_FS;

uniform vec3 colour;
uniform sampler2D image;

uniform float exposure;
uniform float gamma;

void main()
{
	outColour = vec4(colour, 1.0);

	if (outColour.r == -1)
	{
		vec3 col = texture(image, texture_FS).rgb;

		col = 1.0 - exp(-col * exposure);
		col = pow(col, vec3(1.0 / gamma));

		outColour = vec4(col, 1.0);
	}
}