#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>

#include "Renderer.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Renderer/Images/Image.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Pipeline/ShaderProgram.hpp"
#include "Engine/Renderer/Pipeline/Shader.hpp"
#include "Engine/Renderer/Pipeline/RenderState.hpp"
#include "Engine/Renderer/Pipeline/RenderBuffers.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/General/Rgba.hpp"

//===============================================================================================
Renderer* Renderer::s_renderer = nullptr;

D3D_PRIMITIVE_TOPOLOGY PrimitiveTypeConverter[NUM_PRIMITIVE_TYPES] =
{
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};

//===============================================================================================
Renderer::Renderer()
{
	s_renderer = this;
}

//-----------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
	if (m_deviceImmediateContext) m_deviceImmediateContext->ClearState();
	
	if (m_deviceImmediateContext) m_deviceImmediateContext->Release();
	if (m_deviceImmediateContextOne) m_deviceImmediateContextOne->Release();
	if (m_deviceInterface) m_deviceInterface->Release();
	if (m_deviceInterfaceOne) m_deviceInterfaceOne->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_swapChainOne) m_swapChainOne->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_depthStencil) m_depthStencil->Release();
	if (m_depthStencilView) m_depthStencilView->Release();

	delete m_testTexture;
	m_testTexture = nullptr;

	delete m_testShaderProgram;
	m_testShaderProgram = nullptr;

	delete m_tempImmediateIndexBuffer;
	m_tempImmediateIndexBuffer = nullptr;

	delete m_tempImmediateVertexBuffer;
	m_tempImmediateVertexBuffer = nullptr;

	delete m_modelConstantBuffer;
	m_modelConstantBuffer = nullptr;

	delete m_cameraConstantBuffer;
	m_cameraConstantBuffer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Renderer::Startup()
{
	HRESULT hr = S_OK;
	Window* theWindow = Window::GetInstance();
	uint windowWidth = (uint) theWindow->GetWidth();
	uint windowHeight = (uint) theWindow->GetHeight();

	// Creating the device and device context
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_deviceInterface,
		nullptr,
		&m_deviceImmediateContext);

	// We need to get the device factory
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIDevice* dxgiDevice = nullptr;

	// converting our deviceinterface1 into the IDXGIDevice
	hr = m_deviceInterface->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	if (SUCCEEDED(hr))
	{
		IDXGIAdapter* adapter = nullptr; // basically a virtual rep of the video card
		hr = dxgiDevice->GetAdapter(&adapter); // we gotta get the adapter we created with CreateDevice
		if (SUCCEEDED(hr))
		{
			hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
			adapter->Release();
		}
		dxgiDevice->Release();
	}

	// swap chain time
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// We got a factory thats for version 11.1 or later so lets use that to create the chain buffer
		hr = m_deviceInterface->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_deviceInterfaceOne));
		if (SUCCEEDED(hr))
		{
			(void)m_deviceImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_deviceImmediateContextOne));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		// I am pretty sure you could make these 0 and they would take the active window size
		sd.Width = windowWidth;
		sd.Height = windowHeight;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_deviceInterface, (HWND)theWindow->GetHandle(), &sd, nullptr, nullptr, &m_swapChainOne);
		if (SUCCEEDED(hr))
		{
			hr = m_swapChainOne->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_swapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// we got a dx11.0 system
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = windowWidth;
		sd.BufferDesc.Height = windowHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = (HWND)theWindow->GetHandle();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_deviceInterface, &sd, &m_swapChain);
	}

	dxgiFactory->Release();

	// Render target time
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	hr = m_deviceInterface->CreateRenderTargetView(pBackBuffer, nullptr, &m_renderTargetView);
	pBackBuffer->Release();

	// depth buffers
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = windowWidth;
	depthDesc.Height = windowHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	hr = m_deviceInterface->CreateTexture2D(&depthDesc, nullptr, &m_depthStencil);

	// depth target view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_deviceInterface->CreateDepthStencilView(m_depthStencil, &depthStencilViewDesc, &m_depthStencilView);

	// set both the rendertarget and depth buffer
	m_deviceImmediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)theWindow->GetWidth();
	vp.Height = (FLOAT)theWindow->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceImmediateContext->RSSetViewports(1, &vp);

	PostStartup();
}

//-----------------------------------------------------------------------------------------------
void Renderer::PostStartup()
{
	m_testTexture = new Texture("Data/Images/Test_StbiAndDirectX.png");
	
	m_testShaderProgram = new ShaderProgram("TestVertShader", "TestPixelShader");
	RenderState* testRenderState = new RenderState();
	m_testShader = new Shader("TestShader", m_testShaderProgram, testRenderState);

	RenderState* wireFrameRenderState = new RenderState();
	wireFrameRenderState->m_fillMode = FILLMODE_WIRE;
	m_wireFrameShader = new Shader("WireFrame", m_testShaderProgram, wireFrameRenderState);

	m_defaultTexture = new Texture(); //CreateOrGetTexture("Data/Images/defaultTexture.png");
	m_defaultTexture = m_defaultTexture->CreateFromImage(Image("defaultTexture", IntVector2(8, 8), Rgba(255, 255, 255, 255)));

	CameraBufferData cb;
	m_cameraConstantBuffer = new ConstantBuffer(1, sizeof(CameraBufferData), &cb);
	
	ModelBufferData mb;
	m_modelConstantBuffer = new ConstantBuffer(1, sizeof(ModelBufferData), &mb);
}

//-----------------------------------------------------------------------------------------------
void Renderer::BeginFrame()
{
	// this might be needed to go first once we do more complicated things
	//m_deviceImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	float color[4] = { 210.f / 255.f, 74.f / 255.f, 97.f / 255.f, 1.0f };

	m_deviceImmediateContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//-----------------------------------------------------------------------------------------------
void Renderer::EndFrame()
{
	m_swapChain->Present(0, 0);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetCamera(Camera* theCamera)
{
	if(m_currentCamera == nullptr)
	{
		// change current camera to default?
	}

	// do something with the framebuffer?

	m_currentCamera = theCamera;
	BindCameraToShader(*theCamera);
}

//-----------------------------------------------------------------------------------------------
void Renderer::BindCameraToShader(const Camera& theCamera)
{
	CameraBufferData cb;
	cb.m_world = theCamera.m_cameraMatrix;
	cb.m_view = theCamera.m_viewMatrix;
	cb.m_projection = theCamera.m_projectionMatrix;

	UpdateConstantBuffer(m_cameraConstantBuffer, &cb);
	
	SetConstantBuffer(CAMERA_CONSTANT_BUFFER_SLOT, m_cameraConstantBuffer);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetActiveTexture(int slot, const Texture* theTexture)
{
	m_deviceImmediateContext->PSSetShaderResources(slot, 1, &theTexture->m_textureView);
	m_deviceImmediateContext->PSSetSamplers(slot, 1, &theTexture->m_textureSampler);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetActiveShader(const Shader* theShader)
{
	m_deviceImmediateContext->VSSetShader(theShader->m_program->m_vertexShader, nullptr, 0);
	m_deviceImmediateContext->PSSetShader(theShader->m_program->m_pixelShader, nullptr, 0);

	m_deviceImmediateContext->OMSetBlendState(	theShader->m_state->m_blendState, 
												theShader->m_state->m_blendFactor, 
												theShader->m_state->m_mask);

	m_deviceImmediateContext->IASetInputLayout(theShader->m_program->m_vertexLayout);

	theShader->m_state->SetRasterizerState();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetPrimitiveType(ePrimitiveType type)
{
	m_deviceImmediateContext->IASetPrimitiveTopology(PrimitiveTypeConverter[type]);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetVertexBuffer(const VertexBuffer* buffer)
{
	m_deviceImmediateContext->IASetVertexBuffers(0, 1, &buffer->m_buffer, &buffer->m_stride, &buffer->m_offset);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetIndexBuffer(const IndexBuffer* buffer)
{
	m_deviceImmediateContext->IASetIndexBuffer(buffer->m_buffer, DXGI_FORMAT_R16_UINT, buffer->m_offset);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetConstantBuffer(uint slot, const ConstantBuffer* buffer)
{
	m_deviceImmediateContext->VSSetConstantBuffers(slot, 1, &buffer->m_buffer);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetModel(const Matrix44& modelMatrix)
{
	ModelBufferData cb;
	cb.m_model = modelMatrix;

	UpdateConstantBuffer(m_modelConstantBuffer, &cb);

	SetConstantBuffer(MODEL_CONSTANT_BUFFER_SLOT, m_modelConstantBuffer);
}

//-----------------------------------------------------------------------------------------------
void Renderer::UpdateConstantBuffer(const ConstantBuffer* buffer, void* data)
{
	m_deviceImmediateContext->UpdateSubresource(buffer->m_buffer, 0, nullptr, data, 0, 0);
}

//-----------------------------------------------------------------------------------------------
// Requires indicies 
void Renderer::DrawMeshImmediate(ePrimitiveType type, uint vertexCount, VertexMaster* vertices, uint indexCount, Indices* indicies)
{
	if(indicies == nullptr)
	{
		TODO("ERROR check here pls")
		return;
	}
	
	SetPrimitiveType(type);

	// maybe bind current shader and current texture?

	// We are deleting the temp buffers for each draw. Weird issue where either you do this, OR
	// create them with a giant fixed size (like a mil) and then update sub resource 
	// https://stackoverflow.com/questions/26453773/direct3d-c-api-how-to-update-vertex-buffer
	if(m_tempImmediateVertexBuffer != nullptr)
	{
		delete m_tempImmediateVertexBuffer;
		m_tempImmediateVertexBuffer = nullptr;
	}

	if(m_tempImmediateIndexBuffer != nullptr)
	{
		delete m_tempImmediateIndexBuffer;
		m_tempImmediateIndexBuffer = nullptr;
	}

	m_tempImmediateVertexBuffer = new VertexBuffer(vertexCount, sizeof(VertexMaster), vertices);
	m_tempImmediateIndexBuffer = new IndexBuffer(indexCount, sizeof(uint), indicies);

	SetVertexBuffer(m_tempImmediateVertexBuffer);
	SetIndexBuffer(m_tempImmediateIndexBuffer);

	m_deviceImmediateContext->DrawIndexed(indexCount, 0, 0);
}

