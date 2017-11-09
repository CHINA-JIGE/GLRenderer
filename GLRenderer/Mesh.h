
/***********************************************************************

									h��NoiseMesh

************************************************************************/

#pragma once

	class IMesh : private IFileManager
	{
		friend  class IRenderer;

	public:
		//���캯��
		IMesh();

		~IMesh();

		void	CreatePlane(float fWidth, float fDepth, UINT iRowCount = 5, UINT iColumnCount = 5);

		void CreateBox(float fWidth, float fHeight, float fDepth, UINT iDepthStep = 3, UINT iWidthStep = 3, UINT iHeightStep = 3);

		void	CreateSphere(float fRadius, UINT iColumnCount = 20, UINT iRingCount = 20,bool bInvertNormal=false);

		void CreateCylinder(float fRadius, float fHeight, UINT iColumnCount = 40, UINT iRingCount = 8);

		bool	LoadFile_STL(std::string pFilePath);

		bool	LoadFile_OBJ(std::string pFilePath);

		void		SetColor(const VECTOR4& color);

		void		SetMaterial(const Material& mat);

		void		SetTexture(ITexture* pTex);

		void		SetPosition(float x, float y, float z);

		void		SetPosition(const VECTOR3& pos);

		void		SetRotation(float angleX, float angleY, float angleZ);

		void		SetRotationX_Pitch(float angleX);

		void		SetRotationY_Yaw(float angleY);

		void		SetRotationZ_Roll(float angleZ);

		VECTOR3		GetPosition();

		void		RotateX_Pitch(float angle);

		void		RotateY_Yaw(float angle);

		void		RotateZ_Roll(float angle);

		float		GetRotationY_Yaw();

		float		GetRotationX_Pitch();

		float		GetRotationZ_Roll();

	//In order to avoid directly compute inverse of Matrix

		void		GetWorldMatrix(MATRIX4x4& outMat);

		UINT		GetVertexCount();

		void		GetVertex(UINT iIndex, Vertex& outVertex);

		void		GetVertexBuffer(std::vector<Vertex>& outBuff);

		ITexture* GetTexture();

		void		ComputeBoundingBox(BOUNDINGBOX& outBox);

	private:

		void		mFunction_UploadDataToVRAM();

		//invoked by NoiseRenderer
		void		mFunction_UpdateWorldMatrix();

		//this function use the vertex list of vector<Vertex>
		void		mFunction_ComputeBoundingBox();

		void		mFunction_ComputeBoundingBox(std::vector<VECTOR3>* pVertexBuffer);

		VECTOR2	mFunction_ComputeTexCoord_SphericalWrap(VECTOR3 vBoxCenter, VECTOR3 vPoint);

	private:

		//internal mesh generator
		IGeometryMeshGenerator mMeshGenerator;

		VECTOR3							mPosition;
		BOUNDINGBOX				mBoundingBox;

		float									mRotationX_Pitch;
		float									mRotationY_Yaw;
		float									mRotationZ_Roll;


		MATRIX4x4							mMatrixWorld;
		//MATRIX4x4						mMatrixWorldInvTranspose;
		std::vector<Vertex>*			m_pVB_Mem;//vertex in CPU memory
		std::vector<UINT>*				m_pIB_Mem;//index in CPU memory
		GLuint		mVAO;//states encapsulation of VBO
		GLuint		mVBO;//similar to Id3d11Buffer

		Material							mMaterial;
		//graphic attribute
		ITexture*							m_pTexture;

};