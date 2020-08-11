#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>

#include <string>
#include <vector>

#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"

using namespace DirectX;

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct Vertex
{
	Vertex(const Vector3& thePos, const Vector4& theColor)
		: pos(thePos)
		, color(theColor) {}

	Vector3 pos;
	Vector4	color; // normalized
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

//====================================================================================
// Classes
//====================================================================================
class Game
{
public:
	Game();
	~Game();

public: 
	void StartUp();
	void Update(float ds);
	void Render() const;

private:
	bool LoadCompiledShaderFromFile(const std::string& path, int& sizeOut, char*& out);

private:
	ID3D11Device*			m_deviceInterface = nullptr;              // the device interface
	ID3D11Device1*			m_deviceInterfaceOne = nullptr;              // the device interface
	
	ID3D11DeviceContext*	m_deviceImmediateContext = nullptr;    // the device context interface
	ID3D11DeviceContext1*	m_deviceImmediateContextOne = nullptr;    // the device context interface

	IDXGISwapChain*         m_pSwapChain = nullptr;
	IDXGISwapChain1*        m_pSwapChainOne = nullptr;

	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D*        m_depthStencil = nullptr;

	ID3D11VertexShader*		m_vertexShader = nullptr;
	ID3D11PixelShader*		m_pixelShader = nullptr;

	ID3D11InputLayout*		m_vertexLayout = nullptr;
	ID3D11Buffer*			m_vertexBuffer = nullptr;
	ID3D11Buffer*			m_indexBuffer = nullptr;

	// constant buffer for transformations
	ID3D11Buffer*			m_constantBuffer = nullptr;
	XMMATRIX				m_bigCubeWorld;
	XMMATRIX				m_smallCubeWorld;

	XMMATRIX				m_view;
	XMMATRIX				m_projection;
						
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern Game* g_theGame;

//====================================================================================
// Written by Zachary Bracken : [8/2/2020]
//====================================================================================