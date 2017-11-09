#version 330

in vec3 color;
in vec2 texcoord;

//diffuse texture 
uniform sampler2D diffuseMap;

void main()
{
	//component-wise multiplication blending
	gl_FragColor = texture(diffuseMap, texcoord) * vec4(color,1.0f);
}
