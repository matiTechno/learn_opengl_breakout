#version 330

layout (location = 0) in vec4 vertex;

uniform mat4 projection;
uniform mat4 model;

out vec2 texCoords;

void main()
{
	gl_Position = projection * model * vec4(vertex.xy, 0, 1);
	texCoords = vertex.zw;
}
