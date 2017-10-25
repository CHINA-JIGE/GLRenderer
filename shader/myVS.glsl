#version 330

in vec3 inPos;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTexcoord;
out vec3 color;
out vec2 texcoord;

uniform mat4 gWorldMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjMatrix;

void main()
{
	gl_Position = gProjMatrix * gViewMatrix * gWorldMatrix * vec4(inPos,1.0f);//column major
	//note : normal transformation is different. should be inverse transpose!
	//vec3 normalW = normalize(gWorldInvTMatrix * vec4(inNormal,0.0f)).xyz; 
	//pre-set light vector
	/*vec3 lightVec = normalize(vec3(1.0f,-1.0f,0));
	//light-normal angle
	float diffuseIntensity = 3.0f;
	float diffuseFactor = diffuseIntensity* max(dot(normalW,-lightVec),0);
	color = (inColor *diffuseFactor).xyz ;*/
	
	color = inColor.xyz + inNormal*0.0001f;//inNormal exists because i don't want it to be optimized out
	texcoord = inTexcoord;
}
