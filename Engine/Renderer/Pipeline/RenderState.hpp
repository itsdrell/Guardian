#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/RenderTypes.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11BlendState;
struct ID3D11RasterizerState;

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
	void SetBlendState();
	void SetRasterizerState(); // updates resource on gpu

public:
	float	m_blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint	m_mask = 0xffffffff;

public:
	eCullMode	m_cullMode = CULLMODE_BACK;
	eFillMode	m_fillMode = FILLMODE_SOLID;

	eBlendOperation				m_colorBlendOp = COMPARE_ADD;
	eBlendFactor				m_colorSrcFactor = BLEND_SRC_ALPHA;
	eBlendFactor				m_colorDstFactor = BLEND_ONE_MINUS_SRC_ALPHA;

	eBlendOperation				m_alphaBlendOp = COMPARE_ADD;
	eBlendFactor				m_alphaSrcFactor = BLEND_ONE;
	eBlendFactor				m_alphaDstFactor = BLEND_ZERO;

public:
	ID3D11BlendState*		m_blendState = nullptr;
	ID3D11RasterizerState*	m_rasterationState = nullptr;
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