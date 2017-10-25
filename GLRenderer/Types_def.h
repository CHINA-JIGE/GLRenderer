#pragma once

using namespace Math;

enum TIMER_TIMEUINT
{
	TIMER_TIMEUNIT_MILLISECOND = 0,
	TIMER_TIMEUNIT_SECOND = 1,
};

typedef VECTOR3 COLOR3;


struct DirectionalLight
{
public:
	DirectionalLight():
		mAmbientColor(0,0,0),
		mDiffuseColor(0,0,0),
		mSpecularColor(0,0,0),
		mDirection(1.0f,0,0),
		mSpecularIntensity(1.0f),
		mDiffuseIntensity(0.5f),
		mIsEnabled(true)
	{

	}

	bool operator==(DirectionalLight& Light)
	{
		//two memory fragments are identical
		if (memcmp(this, &Light, sizeof(Light)) == 0)
		{
			return true;
		}
		return false;
	}

	VECTOR3 mAmbientColor;	 float		mSpecularIntensity;
	VECTOR3 mDiffuseColor;		float			mDiffuseIntensity;
	VECTOR3 mSpecularColor;	 bool		mIsEnabled;
	VECTOR3 mDirection;
};

struct Material
{
	Material() :
		ambient(0, 0, 0),
		diffuse(1.0f, 0, 0),
		specular(1.0f, 1.0f, 1.0f),
		specularSmoothLevel(10)
	{
	};
	VECTOR3	ambient;
	VECTOR3	diffuse;
	VECTOR3	specular;
	UINT		specularSmoothLevel;
};


struct Vertex
{
	Vertex() {}

	Vertex(Math::VECTOR3 _pos, Math::VECTOR4 _color, Math::VECTOR3 _normal, Math::VECTOR2 _texcoord) :
		pos(_pos), color(_color),normal(_normal), texcoord(_texcoord) {};

	Math::VECTOR3 pos;
	Math::VECTOR4 color;
	Math::VECTOR3 normal;
	Math::VECTOR2 texcoord;
};

struct VertexShaderOutput_Vertex
{
	VECTOR4 posH;//homogenous position
	VECTOR4 color;
	VECTOR2 texcoord;

	//Gouraud shading don't need to pass down the posW and normalW to pixel shader
	//while Phong shading (per-pixel) need it
};

//in OBJ file ,vertex info is composed of indices
struct OBJ_vertexInfoIndex
{
	OBJ_vertexInfoIndex()
	{
		vertexID = texcoordID = vertexNormalID = 0;
	};

	OBJ_vertexInfoIndex(int vID, int texcID, int vnID)
	{
		vertexID = vID;
		texcoordID = texcID;
		vertexNormalID = vnID;
	}

	inline bool operator==(OBJ_vertexInfoIndex const& v)const
	{
		if (vertexID == v.vertexID && texcoordID == v.texcoordID && vertexNormalID == v.vertexNormalID)
		{
			return true;
		}
		return false;
	}

	UINT vertexID;
	UINT texcoordID;
	UINT vertexNormalID;
};
