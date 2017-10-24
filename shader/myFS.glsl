#version 330

in vec3 color;
in vec2 texcoord;


void main()
{
	//gl_FragColor = vec4(1.0f,0,0,1.0f);
	gl_FragColor = vec4(color,texcoord.x);
}
