#version 330

in vec3 inPos;
in vec3 inColor;
in vec2 inTexcoord;
out vec3 color;
out vec2 texcoord;

void main()
{
	texcoord= inTexcoord;
	color =	inColor;
	gl_Position = vec4(inPos,1.0f);
}
