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

	ID3D11VertexShader*		m_vertexShader = nullptr;
	ID3D11PixelShader*		m_pixelShader = nullptr;

	ID3D11InputLayout*		m_vertexLayout = nullptr;
	ID3D11Buffer*			m_vertexBuffer = nullptr;
						
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