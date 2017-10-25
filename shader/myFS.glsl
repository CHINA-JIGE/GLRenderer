#version 330

in vec3 color;
in vec2 texcoord;


void main()
{
	gl_FragColor = vec4(color,texcoord.x);
}
