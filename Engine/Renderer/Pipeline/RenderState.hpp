#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11BlendState;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class RenderState
{

public:

	// Making a default one for now, later we need to add on to this
	RenderState();
	~RenderState();

public:
	float	m_blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint	m_mask = 0xffffffff;

	ID3D11BlendState*		m_blendState = nullptr;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [8/30/2020]
//====================================================================================