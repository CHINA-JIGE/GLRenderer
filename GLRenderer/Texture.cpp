
/************************************************************

				Picture Interface , providing interfaces to 
				2D picture loading and storage.

************************************************************/

#include "pch.h"
#include "RendererGL.h"

ITexture::ITexture()
{
	m_pColorBuffer = new std::vector<COLOR3>;
}

ITexture::~ITexture()
{
	delete m_pColorBuffer;
}

BOOL ITexture::LoadPPM(std::string ppmFilePath)
{
	BOOL yes=IFileManager::ImportFile_PPM(ppmFilePath, mWidth, mHeight, *m_pColorBuffer);
	if (yes == TRUE)
	{
		return TRUE;
	}
	else
	{
		mWidth = 0;
		mHeight = 0;
		ERROR_MSG("ITexture: Load PPM failed!!");
		return FALSE;
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
		m_pColorBuffer->at(index) = color;
	}
}

COLOR3 ITexture::GetPixel(UINT x, UINT y)
{
	if (x<mWidth && y<mHeight)
	{
		UINT index = y*mWidth + x;
		return m_pColorBuffer->at(index);
	}
	return COLOR3(255, 0, 0);
}
