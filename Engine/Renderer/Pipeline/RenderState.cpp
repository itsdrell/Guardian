#include "RenderState.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <d3d11_1.h>

//===============================================================================================
RenderState::RenderState()
{
	Renderer* r = Renderer::GetInstance();
	
	//-----------------------------------------------------------------------------------------------
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-blend-state
	D3D11_BLEND_DESC BlendState = {};
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	r->m_deviceInterface->CreateBlendState(&BlendState, &m_blendState);

	SetRasterizerState();
}

//-----------------------------------------------------------------------------------------------
RenderState::~RenderState()
{
	if (m_blendState) { m_blendState->Release(); }
	if (m_rasterationState) { m_rasterationState->Release(); }
}

//-----------------------------------------------------------------------------------------------
void RenderState::SetRasterizerState()
{
	Renderer* r = Renderer::GetInstance();
	
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.CullMode = ToDx11CullMode(m_cullMode);
	wfdesc.FillMode = ToDx11FillMode(m_fillMode);

	HRESULT hr = r->m_deviceInterface->CreateRasterizerState(&wfdesc, &m_rasterationState);
	r->m_deviceImmediateContext->RSSetState(m_rasterationState);
}

