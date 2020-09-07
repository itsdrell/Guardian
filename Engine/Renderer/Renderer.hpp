#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/RenderTypes.hpp"

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
class ShaderProgram;
class Shader;
class RenderState;

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

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

public:
	void SetActiveTexture(int slot, const Texture* theTexture);
	void SetActiveShader(const Shader* theShader);
	void SetPrimitiveType(ePrimitiveType type);

public:
	void SetVertexBuffer(const VertexBuffer* buffer);
	void SetIndexBuffer(const IndexBuffer* buffer);
	void SetConstantBuffer(uint slot, const ConstantBuffer* buffer);

	void UpdateConstantBuffer(const ConstantBuffer* buffer, void* data);

public:
	void DrawMeshImmediate(ePrimitiveType type, uint vertexCount, VertexMaster* vertices, uint indexCount, Indices* indicies);

private:
	static Renderer* s_renderer;

public:
	ID3D11Device*			m_deviceInterface = nullptr;              // the device interface
	ID3D11Device1*			m_deviceInterfaceOne = nullptr;              // This might be able to just be a temp variable??

	ID3D11DeviceContext*	m_deviceImmediateContext = nullptr;    // the device context interface
	ID3D11DeviceContext1*	m_deviceImmediateContextOne = nullptr;    // This might be able to just be a temp variable??

private:
	IDXGISwapChain*         m_swapChain = nullptr;
	IDXGISwapChain1*        m_swapChainOne = nullptr;

	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D*        m_depthStencil = nullptr;

private:
	VertexBuffer*			m_tempImmediateVertexBuffer = nullptr;
	IndexBuffer*			m_tempImmediateIndexBuffer = nullptr;

public:
	Texture*				m_testTexture = nullptr;
	ShaderProgram*			m_testShaderProgram = nullptr;
	Shader*					m_testShader = nullptr;
	RenderState*			m_testRenderState = nullptr;
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