/*****************************************************

				OpenGL based Renderer

				Author: 练孙鸿
				201536612082
				15软件工程卓越班

*****************************************************/

#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include "glew.h"
#include "freeglut.h"
#include "Math.h"

#define ERROR_MSG(msg) std::cout<<msg<<std::endl;//throw std::exception(std::string(msg).c_str());

struct Vertex
{
	Vertex(){}

	/*Vertex(Math::VECTOR3 _pos, Math::VECTOR3 _color) :
		pos(_pos), color(_color) {};*/

	Vertex(Math::VECTOR3 _pos, Math::VECTOR3 _color, Math::VECTOR2 _texcoord):
		pos(_pos),color(_color),texcoord(_texcoord) {};

	Math::VECTOR3 pos;
	Math::VECTOR3 color;
	Math::VECTOR2 texcoord;
};

class IRenderer
{
public:

	IRenderer();

	~IRenderer();

	bool Init(int argc, char *argv[], DWORD displayMode,int windowPosX,int windowPosY, int windowWidth, int windowHeight, char* title);

	GLuint GetGpuProgramHandle();

	void SetDisplayFunc(void(*CallbackFunc)());

	void SetIdleFunc(void(*CallbackFunc)());

	void	LoadGeometry(const std::vector<Vertex>& vertexList);

	void Clear();

	void Draw(float angle);

	void	Present();

private:

	struct ShaderInfo
	{
		const char*					fileName;//source file
		GLenum						type;//shader type (vs,gs,fs)
		std::vector<GLchar>	sourceBuf;//actual source data buffer
	};

	GLuint		mVSHandle;

	GLuint		mFSHandle;

	GLuint		mGpuProgramHandle;

	GLuint		mVAO;

	GLuint		mVBO;

	std::vector<Vertex>	mVertexList;

	//customized shaders loading
	bool			mFunction_InitShaders();

	GLuint		mFunction_CreateShader(ShaderInfo& s);

	void			mFunction_ReadShaderSource(const char* filePath, std::vector<GLchar>& outSourceBuf);

};

