#version 330

in vec3 inPos;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTexcoord;
out vec3 color;
out vec2 texcoord;
out vec4 posW;
out vec3 normalW;
out vec3 lightVecW;

uniform mat4 gWorldMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjMatrix;



void main()
{
	posW = gWorldMatrix * vec4(inPos,1.0f);
	gl_Position = gProjMatrix * gViewMatrix * posW;//column major
	//note : normal transformation is different. should be inverse transpose!
	//vec3 normalW = normalize(gWorldInvTMatrix * vec4(inNormal,0.0f)).xyz; 
	normalW = normalize(gWorldMatrix * vec4(inNormal,0.0f)).xyz;
	//pre-set light vector
	lightVecW = normalize(vec3(1.0f,-1.0f,0));
	//lighting terms
	float diffuseIntensity = 1.0f;
	float diffuseFactor = diffuseIntensity* max(dot(normalW,-lightVecW),0);

	vec3 ambientColor = vec3(0.2f,0.2f,0.2f);
	vec3 diffuseColor = vec3(1.0f,1.0f,1.0f) * diffuseFactor;

	color = ambientColor+diffuseColor;
	texcoord = inTexcoord;
}
