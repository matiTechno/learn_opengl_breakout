#version 330

in vec2 texCoords;
uniform sampler2D image;
uniform vec4 color;

void main()
{
	gl_FragColor = color * texture(image, texCoords); 
}
