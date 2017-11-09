#pragma once


class ITexture :private IFileManager
{
public:
		
	friend class IRenderer;

	ITexture();

	~ITexture();

	bool		LoadPPM(std::string filePath);

	bool		LoadBMP(std::string filePath);

	UINT	GetWidth();

	UINT	GetHeight();

	void		SetPixel(UINT x, UINT y,const COLOR3& color);

	COLOR3	GetPixel(UINT x, UINT y);

private:

	void		mFunction_CreateGLTexture();

	std::vector<COLOR3> mColorBuffer;
	UINT mWidth;
	UINT mHeight;

	GLuint mTextureHandle;

};