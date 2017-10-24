#version 330

in vec3 inPos;
in vec3 inColor;
in vec2 inTexcoord;
out vec3 color;
out vec2 texcoord;
uniform float angle;

void main()
{
	texcoord= inTexcoord;
	color = inColor;
	gl_Position = vec4(
	inPos.x * cos(angle) - inPos.y * sin(angle),
	inPos.x * sin(angle) + inPos.y * cos(angle),
	0.0f,
	1.0f);
}
