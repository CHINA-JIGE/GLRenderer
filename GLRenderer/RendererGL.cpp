/***********************************************************

						OpenGL based Renderer
	(openGL is a HANDLE-based access library,just like win API)

						Author: 练孙鸿
						201536612082
						15软件工程卓越班

***********************************************************/

#include "pch.h"
#include "RendererGL.h"

GLuint gGpuProgramHandle = -1;

IRenderer::IRenderer():
	m_pMesh(nullptr),
	m_pCamera(nullptr)
{

}

IRenderer::~IRenderer()
{
	glDeleteShader(mFSHandle);
	glDeleteShader(mVSHandle);
	glDeleteProgram(gGpuProgramHandle);
}

bool IRenderer::Init(int argc, char * argv[], DWORD displayMode, int windowPosX, int windowPosY, int windowWidth, int windowHeight, char * title)
{
	glutInit(&argc, argv);

	//RGB single display
	glutInitDisplayMode(displayMode);

	glutInitWindowPosition(windowPosX, windowPosY);

	glutInitWindowSize(windowWidth,windowHeight);

	glutInitContextVersion(4, 3);

	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow(title);

	if (glewInit() != GLEW_OK) { ERROR_MSG("glew init failed!"); return false; }

	mFunction_InitShaders();

	//face culling
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CW);

	//enable Z-Test
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLuint IRenderer::GetGpuProgramHandle()
{
	return gGpuProgramHandle;
}

void IRenderer::SetDisplayFunc(void(*CallbackFunc)())
{
	glutDisplayFunc(CallbackFunc);
}

void IRenderer::SetIdleFunc(void(*CallbackFunc)())
{
	glutIdleFunc(CallbackFunc);
}

void IRenderer::SetTargetMesh(IMesh* pMesh)
{
	m_pMesh = pMesh;
	mVAO = pMesh->mVAO;
}

void IRenderer::SetCamera(ICamera * pCamera)
{
	m_pCamera = pCamera;
}

void IRenderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}

void IRenderer::Render()
{
	if (m_pMesh == nullptr) { ERROR_MSG("IRenderer: mesh hasn't been set!"); }
	if(m_pCamera==nullptr) { ERROR_MSG("IRenderer: camera hasn't been set!"); }

	//parameter setting
	//bind VAO (set saved states in one call) 
	//bind texture
	//use gpu program
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindVertexArray(mVAO);
	glBindTexture(GL_TEXTURE_2D,m_pMesh->GetTexture()->mTextureHandle);
	glUseProgram(gGpuProgramHandle);

	//---------update shader variable
	GLint shaderVar_WorldMat = glGetUniformLocation(gGpuProgramHandle, "gWorldMatrix");
	Math::MATRIX4x4 worldMat;
	m_pMesh->GetWorldMatrix(worldMat);
	glUniformMatrix4fv(shaderVar_WorldMat,1, true, (float*)&worldMat);

	GLint shaderVar_ViewMat = glGetUniformLocation(gGpuProgramHandle, "gViewMatrix");
	Math::MATRIX4x4 viewMat;
	m_pCamera->GetViewMatrix(viewMat);
	glUniformMatrix4fv(shaderVar_ViewMat, 1, true, (float*)&viewMat);

	GLint shaderVar_ProjMat = glGetUniformLocation(gGpuProgramHandle, "gProjMatrix");
	Math::MATRIX4x4 projMat;
	m_pCamera->GetProjMatrix(projMat);
	glUniformMatrix4fv(shaderVar_ProjMat, 1, true, (float*)&projMat);

	GLint shaderVar_EyePos = glGetUniformLocation(gGpuProgramHandle, "gEyePos");
	VECTOR3 camPos = m_pCamera->GetPosition();
	glUniform3f(shaderVar_EyePos, camPos.x,camPos.y,camPos.z);

	//---------texture setting
	glActiveTexture(GL_TEXTURE0);//use texture 0 (note that tex 0 is active by DEFAULT)
	GLint shaderVar_Texture = glGetUniformLocation(gGpuProgramHandle,"diffuseMap" );
	glUniform1i(shaderVar_Texture, 0);//texture No. 0 will be transfered to VRAM if glActiveTexture(GL_TEXTURE0)


	//***********issue draw call***********
	//glDrawArrays(GL_TRIANGLES, 0, m_pTargetVertexList->size());
	std::vector<uint32_t>* pTargertIndexList = m_pMesh->m_pIB_Mem;
	glDrawElements(GL_TRIANGLES, pTargertIndexList->size(), GL_UNSIGNED_INT, &pTargertIndexList->at(0));
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
	glUseProgram(0);
}

void IRenderer::Present()
{
	glutSwapBuffers();
	glutPostRedisplay();
}

/***********************************************

						P R I V A T E

***********************************************/

bool IRenderer::mFunction_InitShaders()
{
	ShaderInfo VSInfo = { "../../shader/myVS.glsl",GL_VERTEX_SHADER };
	ShaderInfo FSInfo = { "../../shader/myFS.glsl",GL_FRAGMENT_SHADER };

	//similar to Technique of D3DX11(Effect11)
	GLuint programHandle = glCreateProgram();

	mVSHandle = mFunction_CreateShader(VSInfo);
	mFSHandle = mFunction_CreateShader(FSInfo);

	//attach shaders to a Gpu Program (like Technique in D3DX11)
	glAttachShader(programHandle, mVSHandle);
	glAttachShader(programHandle, mFSHandle);

	//link program
	glLinkProgram(programHandle);

	//check if program is successfully linked
	GLint programLinked;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &programLinked);
	if (!programLinked)
	{
		ERROR_MSG("GLSL Program linking Error!");
		GLint logSize = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);
		//output compilation log
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(programHandle, logSize, NULL, (GLchar*)logMsg);
		ERROR_MSG( std::string(logMsg));
		return false;
	}

	glUseProgram(programHandle);
	gGpuProgramHandle = programHandle;

	return true;
}

GLuint IRenderer::mFunction_CreateShader(ShaderInfo & s)
{
	mFunction_ReadShaderSource(s.fileName, s.sourceBuf);

	//compile shader with  GLSL source code
	GLuint shaderHandle = glCreateShader(s.type);
	const GLchar* pSourceBuf = &s.sourceBuf.at(0);
	glShaderSource(shaderHandle, 1, &pSourceBuf, nullptr);
	glCompileShader(shaderHandle);

	//check if shader is created successfully
	GLint compiled=0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);

	//error code not null
	if (!compiled)
	{
		ERROR_MSG("Shader Compilation failed!! file name:" + std::string(s.fileName));
		GLint logSize = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logSize);
		//output compilation log
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(shaderHandle, logSize, NULL, (GLchar*)logMsg);
		ERROR_MSG(std::string(logMsg));
		return UINT_MAX;
	}

	//shader created successfully
	return shaderHandle;
}

void IRenderer::mFunction_ReadShaderSource(const char * filePath, std::vector<GLchar>& outSourceBuf)
{
	std::ifstream file(filePath,std::ios::binary);
	if (!file.is_open())
	{
		ERROR_MSG("Shader Source Open failed!! file path:" + std::string(filePath));
		return;
	}

	file.seekg(0, std::ios::end);
	UINT fileSize = file.tellg();
	file.seekg(0);

	//load file, output loaded buffer
	outSourceBuf.resize(fileSize + 1);
	file.read((char*)&outSourceBuf.at(0), fileSize);
	outSourceBuf.at(fileSize) = '\0';
	file.close();
}
