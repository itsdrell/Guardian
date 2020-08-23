#pragma once

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11Device;
struct ID3D11Device1;

struct ID3D11DeviceContext;
struct ID3D11DeviceContext1;

struct IDXGISwapChain;
struct IDXGISwapChain1;

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class Texture;

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
class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	static Renderer* GetInstance() { return s_renderer; }

public:
	void Startup();
	void PostStartup();

public:
	void BeginFrame();
	void EndFrame();


private:
	static Renderer* s_renderer;

// public till we remove everything out of game
public:
	ID3D11Device*			m_deviceInterface = nullptr;              // the device interface
	ID3D11Device1*			m_deviceInterfaceOne = nullptr;              // This might be able to just be a temp variable??

	ID3D11DeviceContext*	m_deviceImmediateContext = nullptr;    // the device context interface
	ID3D11DeviceContext1*	m_deviceImmediateContextOne = nullptr;    // This might be able to just be a temp variable??

	IDXGISwapChain*         m_swapChain = nullptr;
	IDXGISwapChain1*        m_swapChainOne = nullptr;

	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D*        m_depthStencil = nullptr;

public:
	Texture*				m_testTexture = nullptr;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [8/20/2020]
//====================================================================================