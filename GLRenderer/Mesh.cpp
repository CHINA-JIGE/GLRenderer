
/***********************************************************************

							类：NoiseMesh

				简述：封装了一个网格类，由SceneManager来创建   

***********************************************************************/

#include "pch.h"
#include "RendererGL.h"

using namespace Math;

static UINT c_VBstride_Default = sizeof(Vertex);		//VertexBuffer的每个元素的字节跨度
static UINT c_VBoffset = 0;				//VertexBuffer顶点序号偏移 因为从头开始所以offset是0

IMesh::IMesh()
{
	mRotationX_Pitch = 0.0f;
	mRotationY_Yaw = 0.0f;
	mRotationZ_Roll = 0.0f;

	m_pVB_Mem		= new std::vector<Vertex>;
	m_pIB_Mem			= new std::vector<UINT>;

	mMatrixWorld.Identity();

	Material defaultMat;
	defaultMat.ambient = { 0.2f,0.2f,0.2f };
	defaultMat.diffuse	= {0.8f,0.8f,0.8f };
	defaultMat.specular = { 1.0f,1.0f,1.0f };
	defaultMat.specularSmoothLevel = 20;
	IMesh::SetMaterial(defaultMat);

	m_pTexture = nullptr;
}

IMesh::~IMesh()
{
	delete m_pVB_Mem;
	delete m_pIB_Mem;
};

void	IMesh::CreatePlane(float fWidth,float fDepth,UINT iRowCount,UINT iColumnCount)
{
	//check if the input "Step Count" is illegal
	if(iColumnCount <= 2)	{iColumnCount =2;}
	if(iRowCount <= 2)		{iRowCount = 2;}

	m_pVB_Mem->clear();
	m_pIB_Mem->clear();

	//delegate vert/idx creation duty to MeshGenerator 
	mMeshGenerator.CreatePlane(fWidth, fDepth, iRowCount, iColumnCount, *m_pVB_Mem, *m_pIB_Mem);

	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();
};

void IMesh::CreateBox(float fWidth,float fHeight,float fDepth,UINT iDepthStep,UINT iWidthStep,UINT iHeightStep)
{
	m_pVB_Mem->clear();
	m_pIB_Mem->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateBox(fWidth, fHeight, fDepth, iDepthStep, iWidthStep, iHeightStep, *m_pVB_Mem, *m_pIB_Mem);

	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();
}

void	IMesh::CreateSphere(float fRadius,UINT iColumnCount, UINT iRingCount, bool bInvertNormal)
{
	//check if the input "Step Count" is illegal
	if(iColumnCount <= 3)	{iColumnCount =3;}
	if(iRingCount <= 1)		{iRingCount = 1;}

	m_pVB_Mem->clear();
	m_pIB_Mem->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateSphere(fRadius, iColumnCount, iRingCount,bInvertNormal, *m_pVB_Mem, *m_pIB_Mem);

	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();
};

void IMesh::CreateCylinder(float fRadius,float fHeight,UINT iColumnCount,UINT iRingCount)
{
		//check if the input "Step Count" is illegal
	if(iColumnCount <= 3)	{iColumnCount =3;}
	if(iRingCount <= 2)		{iRingCount = 2;}

	m_pVB_Mem->clear();
	m_pIB_Mem->clear();

	//mesh creation delegate to MeshGenerator
	mMeshGenerator.CreateCylinder(fRadius,fHeight, iColumnCount, iRingCount, *m_pVB_Mem, *m_pIB_Mem);

	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();
};

bool IMesh::LoadFile_STL(std::string pFilePath)
{
	//check if buffers have been created
	m_pVB_Mem->clear();
	m_pIB_Mem->clear();


	std::vector<VECTOR3> tmpVertexList;
	std::vector<VECTOR3> tmpNormalList;
	std::string				tmpInfo;
	Vertex				tmpCompleteV;
	VECTOR3			tmpBoundingBoxCenter(0, 0, 0);

	//加载STL
	bool fileLoadSucceeded = false;
	fileLoadSucceeded=IFileManager::ImportFile_STL(pFilePath, tmpVertexList, *m_pIB_Mem, tmpNormalList, tmpInfo);
	if (!fileLoadSucceeded)
	{
		ERROR_MSG("Noise Mesh : Load STL failed!");
		return false;
	}

	//先计算包围盒，就能求出网格的中心点（不一定是Mesh Space的原点）
	mFunction_ComputeBoundingBox(&tmpVertexList);

	//计算包围盒中心点
	tmpBoundingBoxCenter = VECTOR3(
		(mBoundingBox.max.x + mBoundingBox.min.x) / 2.0f,
		(mBoundingBox.max.y + mBoundingBox.min.y) / 2.0f,
		(mBoundingBox.max.z + mBoundingBox.min.z) / 2.0f);



	UINT i = 0;UINT k = 0;
	for (i = 0;i < tmpVertexList.size();i++)
	{
		tmpCompleteV.color = VECTOR4(1.0f, 1.0f, 1.0f,1.0f);
		tmpCompleteV.pos = tmpVertexList.at(i);
		tmpCompleteV.normal = tmpNormalList.at(k);
		tmpCompleteV.texcoord = mFunction_ComputeTexCoord_SphericalWrap(tmpBoundingBoxCenter, tmpCompleteV.pos);
		m_pVB_Mem->push_back(tmpCompleteV);

		//每新增了一个三角形3个顶点 就要轮到下个三角形的法线了
		if (i % 3 == 2) { k++; }
	}

	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();

	return true;
}

bool IMesh::LoadFile_OBJ(std::string pFilePath)
{
	//check if buffers have been created
	m_pVB_Mem->clear();
	m_pIB_Mem->clear();


	std::vector<Vertex> tmpCompleteVertexList;
	std::vector<UINT> tmpNormalList;

	//加载STL
	bool fileLoadSucceeded = false;
	fileLoadSucceeded = IFileManager::ImportFile_OBJ(pFilePath, *m_pVB_Mem, *m_pIB_Mem);
	if (!fileLoadSucceeded)
	{
		ERROR_MSG("Noise Mesh : Load OBJ failed!");
		return false;
	}


	//!!!!!!!!!!!!!!!!!!!!
	mFunction_UploadDataToVRAM();

	return true;
}

void IMesh::SetColor(const VECTOR4& color)
{
	for (auto& v : *m_pVB_Mem)
	{
		v.color = color;
	}
}

void IMesh::SetMaterial(const Material & mat)
{
	mMaterial = mat;
}

void IMesh::SetTexture(ITexture * pTex)
{
	m_pTexture = pTex;
}

void IMesh::SetPosition(float x,float y,float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

void IMesh::SetPosition(const VECTOR3 & pos)
{
	mPosition = pos;
}

void IMesh::SetRotation(float angleX, float angleY, float angleZ)
{
	mRotationX_Pitch	= angleX;
	mRotationY_Yaw		= angleY;
	mRotationZ_Roll		= angleZ;
}

void IMesh::SetRotationX_Pitch(float angleX)
{
	mRotationX_Pitch = angleX;
};

void IMesh::SetRotationY_Yaw(float angleY)
{
	mRotationY_Yaw = angleY;
};

void IMesh::SetRotationZ_Roll(float angleZ)
{
	mRotationZ_Roll = angleZ;
}

VECTOR3 IMesh::GetPosition()
{
	return mPosition;
}

void IMesh::RotateX_Pitch(float angle)
{
	SetRotationX_Pitch(mRotationX_Pitch + angle);
}

void IMesh::RotateY_Yaw(float angle)
{
	SetRotationY_Yaw(mRotationY_Yaw + angle);
}

void IMesh::RotateZ_Roll(float angle)
{
	SetRotationZ_Roll(mRotationZ_Roll + angle);
}

float IMesh::GetRotationY_Yaw()
{
	return mRotationY_Yaw;
}

float IMesh::GetRotationX_Pitch()
{
	return mRotationX_Pitch;
}

float IMesh::GetRotationZ_Roll()
{
	return mRotationZ_Roll;
;
}

void IMesh::GetWorldMatrix(MATRIX4x4 & outMat) 
{
	mFunction_UpdateWorldMatrix();
	outMat = mMatrixWorld;
}

UINT IMesh::GetVertexCount()
{
	return m_pVB_Mem->size();
}

void IMesh::GetVertex(UINT iIndex, Vertex& outVertex)
{
	if (iIndex < m_pVB_Mem->size())
	{
		outVertex = m_pVB_Mem->at(iIndex);
	}
}

void IMesh::GetVertexBuffer(std::vector<Vertex>& outBuff)
{
	std::vector<Vertex>::iterator iterBegin, iterLast;
	iterBegin = m_pVB_Mem->begin();
	iterLast = m_pVB_Mem->end();
	outBuff.assign(iterBegin,iterLast);
}

void IMesh::ComputeBoundingBox(BOUNDINGBOX& outBox)
{
	mFunction_ComputeBoundingBox();
	outBox= mBoundingBox;
}


/***********************************************************************
								PRIVATE					                    
***********************************************************************/

void IMesh::mFunction_UploadDataToVRAM()
{
	//------------UPDATE to buffer

	//vertex array object(save render-related states of current vertex buffer)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int a =sizeof(Vertex);
	//create buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_pVB_Mem->size() * sizeof(Vertex), (void*)&m_pVB_Mem->at(0), GL_STATIC_DRAW);

	//initialize the vertex position attribute from the vertex shader
	GLuint attr1 = glGetAttribLocation(gGpuProgramHandle, "inPos");
	glEnableVertexAttribArray(attr1);
	glVertexAttribPointer(attr1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	GLuint attr2 = glGetAttribLocation(gGpuProgramHandle, "inColor");
	glEnableVertexAttribArray(attr2);
	glVertexAttribPointer(attr2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

	GLuint attr3 = glGetAttribLocation(gGpuProgramHandle, "inNormal");
	glEnableVertexAttribArray(attr3);
	glVertexAttribPointer(attr3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)28);

	GLuint attr4 = glGetAttribLocation(gGpuProgramHandle, "inTexcoord");
	glEnableVertexAttribArray(attr4);
	glVertexAttribPointer(attr4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	mVAO = vao;
	mVBO = vbo;
	//avoid disturbance
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void	IMesh::mFunction_UpdateWorldMatrix()
{
	MATRIX4x4	tmpMatrixScaling;
	MATRIX4x4	tmpMatrixTranslation;
	MATRIX4x4	tmpMatrixRotation;
	MATRIX4x4	tmpMatrix;

	//初始化输出矩阵
	tmpMatrix.Identity();
		
	//旋转矩阵
	tmpMatrix=Matrix_YawPitchRoll(mRotationY_Yaw, mRotationX_Pitch, mRotationZ_Roll);
	
	//修改平移项
	tmpMatrix.m[0][3] = mPosition.x;
	tmpMatrix.m[1][3] = mPosition.y;
	tmpMatrix.m[2][3] = mPosition.z;

	mMatrixWorld = tmpMatrix;
}

void IMesh::mFunction_ComputeBoundingBox()
{
	mFunction_UpdateWorldMatrix();
	//计算包围盒.......重载1

	UINT i = 0;
	VECTOR4 tmpV;
	//遍历所有顶点，算出包围盒3分量均最 小/大 的两个顶点
	for (i = 0;i < m_pVB_Mem->size();i++)
	{
		if (i == 0)
		{
			mBoundingBox.min = m_pVB_Mem->at(0).pos;
			mBoundingBox.max = m_pVB_Mem->at(0).pos;
		}

		//N_DEFAULT_VERTEX
		tmpV = VECTOR4(m_pVB_Mem->at(i).pos.x, m_pVB_Mem->at(i).pos.y, m_pVB_Mem->at(i).pos.z, 1.0f);
		//tmpV = Math::Matrix_Multiply(mMatrixWorld, VECTOR4(tmpV.x,tmpV.y,tmpV.z,1.0f));
		if (tmpV.x <(mBoundingBox.min.x)) { mBoundingBox.min.x = tmpV.x; }
		if (tmpV.y <(mBoundingBox.min.y)) { mBoundingBox.min.y = tmpV.y; }
		if (tmpV.z <(mBoundingBox.min.z)) { mBoundingBox.min.z = tmpV.z; }

		if (tmpV.x >(mBoundingBox.max.x)) { mBoundingBox.max.x = tmpV.x; }
		if (tmpV.y >(mBoundingBox.max.y)) { mBoundingBox.max.y = tmpV.y; }
		if (tmpV.z >(mBoundingBox.max.z)) { mBoundingBox.max.z = tmpV.z; }
	}

	mBoundingBox.max += mPosition;
	mBoundingBox.min += mPosition;
}

void IMesh::mFunction_ComputeBoundingBox(std::vector<VECTOR3>* pVertexBuffer)
{
	mFunction_UpdateWorldMatrix();
	//计算包围盒.......重载1

	UINT i = 0;
	VECTOR4 tmpV;
	//遍历所有顶点，算出包围盒3分量均最 小/大 的两个顶点
	for (i = 0;i < pVertexBuffer->size();i++)
	{
		if (i == 0)
		{
			mBoundingBox.min = pVertexBuffer->at(0);
			mBoundingBox.max = pVertexBuffer->at(0);
		}

		//N_DEFAULT_VERTEX
		tmpV = VECTOR4(pVertexBuffer->at(i).x, pVertexBuffer->at(i).y, pVertexBuffer->at(i).z, 1.0f);
		//tmpV = Math::Matrix_Multiply(mMatrixWorld, VECTOR4(tmpV.x,tmpV.y,tmpV.z,1.0f));
		if (tmpV.x <(mBoundingBox.min.x)) { mBoundingBox.min.x = tmpV.x; }
		if (tmpV.y <(mBoundingBox.min.y)) { mBoundingBox.min.y = tmpV.y; }
		if (tmpV.z <(mBoundingBox.min.z)) { mBoundingBox.min.z = tmpV.z; }

		if (tmpV.x >(mBoundingBox.max.x)) { mBoundingBox.max.x = tmpV.x; }
		if (tmpV.y >(mBoundingBox.max.y)) { mBoundingBox.max.y = tmpV.y; }
		if (tmpV.z >(mBoundingBox.max.z)) { mBoundingBox.max.z = tmpV.z; }
	}

	mBoundingBox.max += mPosition;
	mBoundingBox.min += mPosition;
}


inline VECTOR2 IMesh::mFunction_ComputeTexCoord_SphericalWrap(VECTOR3 vBoxCenter, VECTOR3 vPoint)
{
	//额...这个函数做简单的纹理球形包裹

	VECTOR2 outTexCoord(0,0);
	VECTOR3 tmpP= vPoint - vBoxCenter;

	//投影到单位球上
	tmpP.Normalize();

	//反三角函数算球坐标系坐标，然后角度值映射到[0,1]
	float angleYaw = 0.0f;
	float anglePitch = 0.0f;
	float tmpLength = sqrtf(tmpP.x*tmpP.x + tmpP.z*tmpP.z);

	// [ -PI/2 , PI/2 ]
	anglePitch = atan2(tmpP.y,tmpLength);

	// [ -PI	, PI ]
	angleYaw =	atan2(tmpP.z, tmpP.x);	

	//map to [0,1]
	outTexCoord.x = (angleYaw +  Math::PI) / (2.0f * Math::PI);
	outTexCoord.y = (anglePitch + (Math::PI /2.0f) ) / Math::PI;

	return outTexCoord;
};
