/***********************************************************

						OpenGL based Renderer
	(openGL is a HANDLE-based access library,just like win API)

						Author: 练孙鸿
						201536612082
						15软件工程卓越班

***********************************************************/

#include "RendererGL.h"

IRenderer::IRenderer()
{
	mGpuProgramHandle = 0;
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

void IRenderer::LoadGeometry()
{
	//load vertices
	Vertex vertices[3] =
	{
		Vertex({-0.5f,-0.8f,0.0f},{0.1f,0.2f,0.8f},{1.0f,0.0f}),
		Vertex({ 0.7f,-0.6f,0.0f },{ 1.0f,0.2f,0.2f },{ 1.0f,1.0f }),
		Vertex({ 0.0f,0.6f,0.0f },{ 0.1f,0.9f,0.2f },{ 1.0f,0.0f })
	};

	/*Math::VECTOR3 vertices[3] =
	{
		{0.5f,-0.6f,1.0f },
		{-0.5f,-0.6f,1.0f },
		{ 0.0f,0.6f,1.0f }
	};*/
	

	//vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);

	//initialize the vertex position attribute from the vertex shader
	GLuint attr1 =  glGetAttribLocation(mGpuProgramHandle, "inPos");
	glEnableVertexAttribArray(attr1);
	glVertexAttribPointer(attr1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 

	GLuint attr2 =  glGetAttribLocation(mGpuProgramHandle, "inColor");
	glEnableVertexAttribArray(attr2);
	glVertexAttribPointer(attr2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

	GLuint attr3 = glGetAttribLocation(mGpuProgramHandle, "inTexcoord");
	glEnableVertexAttribArray(attr3);
	glVertexAttribPointer(attr3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);


	mVAO = vao;
	mVBO = vbo;
	//avoid disturbance
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void IRenderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
}

void IRenderer::Draw()
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void IRenderer::Present()
{
	glFlush();
}

/***********************************************

						P R I V A T E

***********************************************/

bool IRenderer::mFunction_InitShaders()
{
	ShaderInfo VSInfo = { "../shader/myVS.glsl",GL_VERTEX_SHADER };
	ShaderInfo FSInfo = { "../shader/myFS.glsl",GL_FRAGMENT_SHADER };

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
