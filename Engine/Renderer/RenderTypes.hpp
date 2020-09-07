#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


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


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/6/2020]
//====================================================================================