#version 330

in vec3 color;
in vec2 texcoord;
in vec4 posW;
in vec3 normalW;
in vec3 lightVecW;

uniform vec3 gEyePos;
//diffuse texture 
uniform sampler2D diffuseMap;

void main()
{
	float specularIntensity = 5.0f;
	int specularSmoothLevel = 20;
	float specularFactor = specularIntensity * pow(max(dot(reflect(lightVecW,normalW),normalize(gEyePos-posW.xyz)),0),specularSmoothLevel);
	vec3 specularColor = vec3(1.0f,1.0f,1.0f) * specularFactor;
	
	//component-wise multiplication blending
	gl_FragColor = texture(diffuseMap, texcoord) * vec4(color+specularColor,1.0f);
}
