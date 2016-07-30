#version 330

in vec2 texCoords;
uniform sampler2D image;
uniform vec3 color;

void main()
{
	gl_FragColor = vec4(color, 1) * texture(image, texCoords); 
}
