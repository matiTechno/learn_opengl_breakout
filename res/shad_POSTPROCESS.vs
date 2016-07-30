#version 330

layout (location = 0) in vec4 vertex;

uniform bool shake;
uniform bool invert;
uniform bool edge_det;
uniform float time;

out vec2 texCoords;

void main()
{
	gl_Position = vec4(vertex.xy, 0, 1);
	vec2 texture = vertex.zw;

	if(edge_det)
	{
		float strength = 0.3;
		vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
		texCoords = pos;
	}
	else if(invert)
	{
		texCoords = vec2(1 - texture.x, 1 - texture.y);
	}
	else
	{
		texCoords = texture;
	}
	if(shake)
	{
		float strength = 0.005;
		gl_Position.x += cos(time*30) * strength;
		gl_Position.y += cos(time*50) * strength;
	}
}