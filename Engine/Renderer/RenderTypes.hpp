#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Matrices/Matrix44.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
enum D3D11_CULL_MODE;
enum D3D11_FILL_MODE;
enum D3D11_BLEND;
enum D3D11_BLEND_OP;

//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef uint16 Indices; // needs to be this type for some reason

//====================================================================================
// ENUMS
//====================================================================================
enum ePrimitiveType
{
	PRIMITIVE_POINTS,			// in d3d11, for example, this becomes D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	PRIMITIVE_LINES,			// in d3d11, for example, this becomes D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	PRIMITIVE_TRIANGLES,		// in d3d11, for example, this becomes D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	NUM_PRIMITIVE_TYPES
};

//-----------------------------------------------------------------------------------------------
enum eCullMode
{
	CULLMODE_BACK,          // D3D11_CULL_BACK 
	CULLMODE_FRONT,         // D3D11_CULL_FRONT
	CULLMODE_NONE           // D3D11_CULL_NONE 
};
D3D11_CULL_MODE ToDx11CullMode(eCullMode mode);

//-----------------------------------------------------------------------------------------------
enum eFillMode
{
	FILLMODE_SOLID,         // D3D11_FILL_SOLID
	FILLMODE_WIRE           // D3D11_FILL_WIREFRAME
};
D3D11_FILL_MODE ToDx11FillMode(eFillMode mode);

//-----------------------------------------------------------------------------------------------
enum eBlendOperation
{
	COMPARE_ADD,					// D3D11_BLEND_OP_ADD
	COMPARE_SUBTRACT,				// D3D11_BLEND_OP_SUBTRACT
	COMPARE_REVERSE_SUBTRACT,		// D3D11_BLEND_OP_REV_SUBTRACT
	COMPARE_MIN,					// D3D11_BLEND_OP_MIN
	COMPARE_MAX						// D3D11_BLEND_OP_MAX
};
enum D3D11_BLEND_OP ToDx11BlendOperation(eBlendOperation operation);

//-----------------------------------------------------------------------------------------------
enum eBlendFactor
{
	BLEND_ZERO,						   //D3D11_BLEND_ZERO,
	BLEND_ONE,						   //D3D11_BLEND_ONE,
	BLEND_SRC_COLOR,				   //D3D11_BLEND_SRC_COLOR,
	BLEND_ONE_MINUS_SRC_COLOR,		   //D3D11_BLEND_INV_SRC_COLOR,
	BLEND_DST_COLOR,				   //D3D11_BLEND_DEST_COLOR,
	BLEND_ONE_MINUS_DST_COLOR,		   //D3D11_BLEND_INV_DEST_COLOR,
	BLEND_SRC_ALPHA,				   //D3D11_BLEND_SRC_ALPHA,
	BLEND_ONE_MINUS_SRC_ALPHA,		   //D3D11_BLEND_INV_SRC_ALPHA,
	BLEND_DST_ALPHA,				   //D3D11_BLEND_DEST_ALPHA,
	BLEND_ONE_MINUS_DST_ALPHA,		   //D3D11_BLEND_INV_DEST_ALPHA,
	BLEND_SRC_ALPHA_SATURATE		   //D3D11_BLEND_SRC_ALPHA_SAT
	//BLEND_CONSTANT_COLOR,					//GL_CONSTANT_COLOR,
	//BLEND_ONE_MINUS_CONSTANT_COLOR,		//GL_ONE_MINUS_CONSTANT_COLOR,
	//BLEND_CONSTANT_ALPHA,					//GL_CONSTANT_ALPHA,
	//BLEND_ONE_MINUS_CONSTANT_ALPHA,		//GL_ONE_MINUS_CONSTANT_ALPHA,

};
D3D11_BLEND ToDx11BlendFactor(eBlendFactor factor);

//-----------------------------------------------------------------------------------------------
enum RENDER_CONSTANT_BUFFER_SLOTS
{
	MODEL_CONSTANT_BUFFER_SLOT = 1,
	CAMERA_CONSTANT_BUFFER_SLOT = 0,
};

//====================================================================================
// Structs
//====================================================================================
struct VertexMaster
{
	VertexMaster(const Vector3& thePos, const Vector4& theColor, const Vector2& theUvs)
		: pos(thePos)
		, color(theColor)
		, uv(theUvs) {}

	Vector3 pos;
	Vector4	color; // normalized
	Vector2 uv;
};

//-----------------------------------------------------------------------------------------------
// TODO PUT MODEL IN ITS OWN
struct CameraBufferData
{
	Matrix44	m_world;
	Matrix44	m_view;
	Matrix44	m_projection;
};

//-----------------------------------------------------------------------------------------------
struct ModelBufferData
{
	Matrix44 m_model;
};


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/6/2020]
//====================================================================================