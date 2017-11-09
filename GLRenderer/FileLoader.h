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
		WORD bfType;//�̶�Ϊ0x4d42
		DWORD bfSize; //�ļ���С
		WORD bfReserved1; //�����֣�������
		WORD bfReserved2; //�����֣�ͬ��
		DWORD bfOffBits; //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
	} BITMAPFILEHEADER;

	typedef struct tagBITMAPINFOHEADER {
		//public:
		DWORD biSize; //ָ���˽ṹ��ĳ��ȣ�Ϊ40
		LONG biWidth; //λͼ��
		LONG biHeight; //λͼ��
		WORD biPlanes; //ƽ������Ϊ1
		WORD biBitCount; //������ɫλ����������1��2��4��8��16��24���µĿ�����32
		DWORD biCompression; //ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
		DWORD biSizeImage; //ʵ��λͼ����ռ�õ��ֽ���
		LONG biXPelsPerMeter; //X����ֱ���
		LONG biYPelsPerMeter; //Y����ֱ���
		DWORD biClrUsed; //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
		DWORD biClrImportant; //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
	} BITMAPINFOHEADER;


	//��ɫ��Palette����Ȼ�������Ƕ���Щ��Ҫ��ɫ���λͼ�ļ����Եġ�24λ��32λ�ǲ���Ҫ��ɫ��ġ�
	//���ƺ��ǵ�ɫ��ṹ���������ʹ�õ���ɫ������
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