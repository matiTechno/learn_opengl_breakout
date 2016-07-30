#version 330

in vec2 texCoords;
out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool shake;
uniform bool invert;
uniform bool edge_det;

void main()
{
	color = vec4(0);
	vec3 sample[9];

	if(edge_det || shake)
		for(int i = 0; i < 9; i++)
			sample[i] = vec3(texture(scene, texCoords.st + offsets[i]));

	if(edge_det)
	{
		for(int i = 0; i < 9; i++)
			color += vec4(sample[i] * edge_kernel[i], 0);
		color.a = 1;
	}
	else if(invert)
	{
		color = vec4(1 - texture(scene, texCoords).rgb, 1);
	}
	else if(shake)
	{
		for(int i = 0; i < 9; i++)
			color += vec4(sample[i] * blur_kernel[i], 0);
		color.a = 1;
	}
	else
	{
		color = texture(scene, texCoords);
	}
}