/***********************************************************

						OpenGL based Renderer
	(openGL is a HANDLE-based access library,just like win API)

						Author: 练孙鸿
						201536612082
						15软件工程卓越班

***********************************************************/

#include "pch.h"
#include "RendererGL.h"

IRenderer::IRenderer():
	m_pMesh(nullptr),
	m_pCamera(nullptr),
	mGpuProgramHandle(-1)
{

}

IRenderer::~IRenderer()
{
	glDeleteShader(mFSHandle);
	glDeleteShader(mVSHandle);
	glDeleteProgram(mGpuProgramHandle);
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
	return mGpuProgramHandle;
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
	std::vector<Vertex>* pTargetVertexList = pMesh->m_pVB_Mem;
	std::vector<uint32_t>* pTargertIndexList = pMesh->m_pIB_Mem;

	//vertex array object(save render-related states of current vertex buffer)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pTargetVertexList->size() * sizeof(Vertex), (void*)&pTargetVertexList->at(0), GL_STATIC_DRAW);

	//initialize the vertex position attribute from the vertex shader
	GLuint attr1 =  glGetAttribLocation(mGpuProgramHandle, "inPos");
	glEnableVertexAttribArray(attr1);
	glVertexAttribPointer(attr1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 

	GLuint attr2 =  glGetAttribLocation(mGpuProgramHandle, "inColor");
	glEnableVertexAttribArray(attr2);
	glVertexAttribPointer(attr2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

	GLuint attr3 = glGetAttribLocation(mGpuProgramHandle, "inNormal");
	glEnableVertexAttribArray(attr3);
	glVertexAttribPointer(attr3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)28);

	GLuint attr4 = glGetAttribLocation(mGpuProgramHandle, "inTexcoord");
	glEnableVertexAttribArray(attr3);
	glVertexAttribPointer(attr3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	mVAO = vao;
	mVBO = vbo;
	//avoid disturbance
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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

	//bind VAO (set saved states in one call)
	glBindVertexArray(mVAO);
	glUseProgram(mGpuProgramHandle);

	//update shader variable
	GLint shaderVar_WorldMat = glGetUniformLocation(mGpuProgramHandle, "gWorldMatrix");
	Math::MATRIX4x4 worldMat;
	m_pMesh->GetWorldMatrix(worldMat);
	glUniformMatrix4fv(shaderVar_WorldMat,1, true, (float*)&worldMat);

	GLint shaderVar_ViewMat = glGetUniformLocation(mGpuProgramHandle, "gViewMatrix");
	Math::MATRIX4x4 viewMat;
	m_pCamera->GetViewMatrix(viewMat);
	glUniformMatrix4fv(shaderVar_ViewMat, 1, true, (float*)&viewMat);

	GLint shaderVar_ProjMat = glGetUniformLocation(mGpuProgramHandle, "gProjMatrix");
	Math::MATRIX4x4 projMat;
	m_pCamera->GetProjMatrix(projMat);
	glUniformMatrix4fv(shaderVar_ProjMat, 1, true, (float*)&projMat);

	//***********issue draw call***********
	//glDrawArrays(GL_TRIANGLES, 0, m_pTargetVertexList->size());
	glEnable(GL_CULL_FACE);
	std::vector<uint32_t>* pTargertIndexList = m_pMesh->m_pIB_Mem;
	glDrawElements(GL_TRIANGLES, pTargertIndexList->size(), GL_UNSIGNED_INT, &pTargertIndexList->at(0));
	glBindVertexArray(0);
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
	mGpuProgramHandle = programHandle;

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
