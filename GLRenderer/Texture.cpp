
/************************************************************

				Picture Interface , providing interfaces to 
				2D picture loading and storage.

************************************************************/

#include "pch.h"
#include "RendererGL.h"

ITexture::ITexture():
	mTextureHandle(0)
{

}

ITexture::~ITexture()
{
	glDeleteTextures(1, &mTextureHandle);
}

bool ITexture::LoadPPM(std::string ppmFilePath)
{
	bool yes=IFileManager::ImportFile_PPM(ppmFilePath, mWidth, mHeight, mColorBuffer);
	if (yes)
	{
		mFunction_CreateGLTexture();
		return true;
	}
	else
	{
		mWidth = 0;
		mHeight = 0;
		ERROR_MSG("ITexture: Load PPM failed!!");
		return false;
	}
}

bool ITexture::LoadBMP(std::string filePath)
{
	bool yes = IFileManager::ImportFile_BMP(filePath, mWidth, mHeight, mColorBuffer);
	if (yes)
	{
		mFunction_CreateGLTexture();
		return true;
	}
	else
	{
		mWidth = 0;
		mHeight = 0;
		ERROR_MSG("ITexture: Load BMP failed!!");
		return false;
	}
}

UINT ITexture::GetWidth()
{
	return mWidth;
}

UINT ITexture::GetHeight()
{
	return mHeight;
}

inline void ITexture::SetPixel(UINT x, UINT y,const COLOR3& color)
{
	if (x<mWidth && y<mHeight)
	{
		UINT index = y*mWidth + x;
		mColorBuffer.at(index) = color;
	}
}

COLOR3 ITexture::GetPixel(UINT x, UINT y)
{
	if (x<mWidth && y<mHeight)
	{
		UINT index = y*mWidth + x;
		return mColorBuffer.at(index);
	}
	return COLOR3(0, 0, 0);
}

/*********************************************

						P R I V A T E 

*********************************************/

void ITexture::mFunction_CreateGLTexture()
{
	glGenTextures(1, &mTextureHandle);
	glBindTexture(GL_TEXTURE_2D,mTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST

	//"level" parameter stands for mipmap level, 0 for disabling mipmaping
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, mWidth, mHeight, 0, GL_RGB,GL_FLOAT,&mColorBuffer.at(0) );

	//clear bound texture
	glBindTexture(GL_TEXTURE_2D, 0);
}
