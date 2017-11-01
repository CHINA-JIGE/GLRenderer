/*****************************************************

				OpenGL based Renderer

				Author: 练孙鸿
				201536612082
				15软件工程卓越班

*****************************************************/

#pragma once

//NDEBUG def is relevant to linking stage.
//if NDEBUG is not defined, "freeglutd.lib" will be linked

#include "pch.h"
#include "FileLoader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"

#define ERROR_MSG(msg) std::cout<<msg<<std::endl;//throw std::exception(std::string(msg).c_str());

extern GLuint	gGpuProgramHandle;//gpu program handle

class IRenderer
{
public:

	IRenderer();

	~IRenderer();

	bool Init(int argc, char *argv[], DWORD displayMode,int windowPosX,int windowPosY, int windowWidth, int windowHeight, char* title);

	GLuint GetGpuProgramHandle();

	void SetDisplayFunc(void(*CallbackFunc)());

	void SetIdleFunc(void(*CallbackFunc)());

	void	SetTargetMesh(IMesh* pMesh);

	void	SetCamera(ICamera* pCamera);

	void Clear();

	void Render();

	void	Present();

private:

	struct ShaderInfo
	{
		const char*					fileName;//source file
		GLenum						type;//shader type (vs,gs,fs)
		std::vector<GLchar>	sourceBuf;//actual source data buffer
	};

	GLuint		mVSHandle;//vertrex shader

	GLuint		mFSHandle;//fragment shader

	GLuint		mVAO;//states encapsulation of VBO

	GLuint		mVBO;

	IMesh*		m_pMesh;

	ICamera*	m_pCamera;

	//customized shaders loading
	bool			mFunction_InitShaders();

	GLuint		mFunction_CreateShader(ShaderInfo& s);

	void			mFunction_ReadShaderSource(const char* filePath, std::vector<GLchar>& outSourceBuf);

};

