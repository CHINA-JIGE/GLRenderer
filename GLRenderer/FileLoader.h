#pragma once

class IFileManager
{
public:
	bool ImportFile_STL(std::string filePath, std::vector<VECTOR3>& refVertexBuffer, std::vector<UINT>& refIndexBuffer, std::vector<VECTOR3>& refNormalBuffer, std::string & refFileInfo);

	bool ImportFile_OBJ(std::string pFilePath, std::vector<Vertex>& refVertexBuffer, std::vector<UINT>& refIndexBuffer);

	bool ImportFile_PPM(std::string filePath, UINT& outWidth,UINT& outHeight,std::vector<COLOR3>& outColorBuffer);

	bool	ImportFile_BMP(std::string filePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

private:

	typedef struct tagBITMAPFILEHEADER 
	{
		WORD bfType;//固定为0x4d42
		DWORD bfSize; //文件大小
		WORD bfReserved1; //保留字，不考虑
		WORD bfReserved2; //保留字，同上
		DWORD bfOffBits; //实际位图数据的偏移字节数，即前三个部分长度之和
	} BITMAPFILEHEADER;

	typedef struct tagBITMAPINFOHEADER {
		//public:
		DWORD biSize; //指定此结构体的长度，为40
		LONG biWidth; //位图宽
		LONG biHeight; //位图高
		WORD biPlanes; //平面数，为1
		WORD biBitCount; //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
		DWORD biCompression; //压缩方式，可以是0，1，2，其中0表示不压缩
		DWORD biSizeImage; //实际位图数据占用的字节数
		LONG biXPelsPerMeter; //X方向分辨率
		LONG biYPelsPerMeter; //Y方向分辨率
		DWORD biClrUsed; //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
		DWORD biClrImportant; //重要颜色数，如果为0，则表示所有颜色都是重要的
	} BITMAPINFOHEADER;


	//调色板Palette，当然，这里是对那些需要调色板的位图文件而言的。24位和32位是不需要调色板的。
	//（似乎是调色板结构体个数等于使用的颜色数。）
	typedef struct tagRGBQUAD {
		//public:
		BYTE rgbBlue;
		BYTE rgbGreen;
		BYTE rgbRed;
		BYTE rgbReserved;
	} RGBQUAD;

	bool mFunction_ImportFile_STL_Binary(std::string filePath, std::vector<VECTOR3>& refVertexBuffer, std::vector<UINT>& refIndexBuffer, std::vector<VECTOR3>& refNormalBuffer, std::string& refFileInfo);

	bool mFunction_ImportFile_STL_Ascii(std::string pFilePath, std::vector<VECTOR3>& refVertexBuffer, std::vector<UINT>& refIndexBuffer, std::vector<VECTOR3>& refNormalBuffer, std::string& refFileInfo);

};