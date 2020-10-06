#include "RenderState.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <d3d11_1.h>

//===============================================================================================
RenderState::RenderState()
{
	SetBlendState();
	SetRasterizerState();
}

//-----------------------------------------------------------------------------------------------
RenderState::~RenderState()
{
	if (m_blendState) { m_blendState->Release(); }
	if (m_rasterationState) { m_rasterationState->Release(); }
}

//-----------------------------------------------------------------------------------------------
void RenderState::SetBlendState()
{
	Renderer* r = Renderer::GetInstance();
	
	//-----------------------------------------------------------------------------------------------
	// https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-blend-state
	D3D11_BLEND_DESC BlendState = {};
	BlendState.RenderTarget[0].BlendEnable = TRUE;

	BlendState.RenderTarget[0].BlendOp = ToDx11BlendOperation(m_colorBlendOp);
	BlendState.RenderTarget[0].SrcBlend = ToDx11BlendFactor(m_colorSrcFactor);
	BlendState.RenderTarget[0].DestBlend = ToDx11BlendFactor(m_colorDstFactor);	

	BlendState.RenderTarget[0].BlendOpAlpha = ToDx11BlendOperation(m_alphaBlendOp);
	BlendState.RenderTarget[0].SrcBlendAlpha = ToDx11BlendFactor(m_alphaSrcFactor);
	BlendState.RenderTarget[0].DestBlendAlpha = ToDx11BlendFactor(m_alphaDstFactor);

	BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	r->m_deviceInterface->CreateBlendState(&BlendState, &m_blendState);
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