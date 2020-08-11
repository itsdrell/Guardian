#include "Game.hpp"
#include "Engine/Core/Platform/Window.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>

#include <directxmath.h>

//===============================================================================================
Game* g_theGame = nullptr;
//===============================================================================================
Game::Game()
{
	g_theGame = this;

	StartUp();
}

//-----------------------------------------------------------------------------------------------
Game::~Game()
{
	if (m_deviceImmediateContext) m_deviceImmediateContext->ClearState();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pSwapChainOne) m_pSwapChainOne->Release();
	if (m_deviceImmediateContext) m_deviceImmediateContext->Release();
	if (m_deviceImmediateContextOne) m_deviceImmediateContextOne->Release();
	if (m_deviceInterface) m_deviceInterface->Release();
	if (m_deviceInterfaceOne) m_deviceInterfaceOne->Release();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_vertexShader->Release();
	if (m_vertexLayout) m_vertexLayout->Release();
	if (m_vertexBuffer) m_vertexBuffer->Release();

	if (m_depthStencil) m_depthStencil->Release();
	if (m_depthStencilView) m_depthStencilView->Release();
}

//-----------------------------------------------------------------------------------------------
void Game::StartUp()
{
	HRESULT hr = S_OK;
	Window* theWindow = Window::GetInstance();
	
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
	if(dxgiFactory2)
	{
		// We got a factory thats for version 11.1 or later so lets use that to create the chain buffer
		hr = m_deviceInterface->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_deviceInterfaceOne));
		if(SUCCEEDED(hr))
		{
			(void)m_deviceImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_deviceImmediateContextOne));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		// I am pretty sure you could make these 0 and they would take the active window size
		sd.Width = theWindow->GetWidth();
		sd.Height = theWindow->GetHeight();
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_deviceInterface, (HWND) theWindow->GetHandle(), &sd, nullptr, nullptr, &m_pSwapChainOne);
		if(SUCCEEDED(hr))
		{
			hr = m_pSwapChainOne->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// we got a dx11.0 system
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = theWindow->GetWidth();
		sd.BufferDesc.Height = theWindow->GetHeight();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = (HWND) theWindow->GetHandle();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_deviceInterface, &sd, &m_pSwapChain);
	}

	dxgiFactory->Release();

	// Render target time
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	hr = m_deviceInterface->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();

	// depth buffers
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = theWindow->GetWidth();
	depthDesc.Height = theWindow->GetHeight();
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
	m_deviceImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_depthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT) theWindow->GetWidth();
	vp.Height = (FLOAT) theWindow->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceImmediateContext->RSSetViewports(1, &vp);

	//-----------------------------------------------------------------------------------------------
	// Create pixel shader
	int pixelShaderlength = 0;
	char* pixelShaderBuffer = nullptr;
	
	bool result = LoadCompiledShaderFromFile("Data\\Shaders\\Compiled\\TestPixelShader.cso", pixelShaderlength, pixelShaderBuffer);
	if(result == false)
	{
		return;
	}
	
	hr = m_deviceInterface->CreatePixelShader(pixelShaderBuffer, pixelShaderlength, nullptr, &m_pixelShader);

	//-----------------------------------------------------------------------------------------------
	// create vertex shader
	int vertexShaderLength = 0;
	char* vertexShaderBuffer = nullptr;
	result = LoadCompiledShaderFromFile("Data\\Shaders\\Compiled\\TestVertShader.cso", vertexShaderLength, vertexShaderBuffer);

	if (result == false)
	{
		return;
	}
	
	hr = m_deviceInterface->CreateVertexShader(vertexShaderBuffer, vertexShaderLength, nullptr, &m_vertexShader);

	// input layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = m_deviceInterface->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer, vertexShaderLength, &m_vertexLayout);
	m_deviceImmediateContext->IASetInputLayout(m_vertexLayout);

	//-----------------------------------------------------------------------------------------------
	// set the shaders as active
	m_deviceImmediateContext->VSSetShader(m_vertexShader, nullptr, 0);
	m_deviceImmediateContext->PSSetShader(m_pixelShader, nullptr, 0);
	
	free(pixelShaderBuffer);
	free(vertexShaderBuffer);

	//-----------------------------------------------------------------------------------------------
	// Create the vertex buffer
	Vertex vertices[] =
	{
		Vertex(Vector3(-1.0f,  1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(Vector3(1.0f,  1.0f, -1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) ),
		Vertex(Vector3(1.0f,  1.0f,  1.0f),		Vector4(0.0f, 1.0f, 1.0f, 1.0f) ),
		Vertex(Vector3(-1.0f,  1.0f,  1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 0.0f, 1.0f) ),
		Vertex(Vector3(1.0f, -1.0f,  1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) ),
		Vertex(Vector3(-1.0f, -1.0f,  1.0f),	Vector4(0.0f, 0.0f, 0.0f, 1.0f)),
	};

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;

	hr = m_deviceInterface->CreateBuffer(&bufferDescription, &InitData, &m_vertexBuffer);

	// I feel like this part should happen in rendering every frame, but doing it once we 
	// create the buffer could be a nice sanity check (or waste)
	// should def be a function we call before drawing
	uint stride = sizeof(Vertex);
	uint offset = 0;
	m_deviceImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//-----------------------------------------------------------------------------------------------
	// Index buff creation
	// Create index buffer
	uint16 indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = sizeof(uint16) * 36; // 36 vertices needed for 12 triangles in a triangle list
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	InitData.pSysMem = indices;
	
	hr = m_deviceInterface->CreateBuffer(&indexBufferDescription, &InitData, &m_indexBuffer);
	m_deviceImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//-----------------------------------------------------------------------------------------------
	m_deviceImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//-----------------------------------------------------------------------------------------------
	// Constant buffer
	D3D11_BUFFER_DESC constantBufferDescription = {};
	constantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDescription.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = 0;

	hr = m_deviceInterface->CreateBuffer(&constantBufferDescription, nullptr, &m_constantBuffer);

	// init the constant variable
	m_bigCubeWorld = XMMatrixIdentity();
	m_smallCubeWorld = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, theWindow->GetAspect(), 0.01f, 100.0f);

}

//-----------------------------------------------------------------------------------------------
void Game::Update(float ds)
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;
	
	m_bigCubeWorld = XMMatrixRotationY(t);


	// 2nd Cube:  Rotate around origin
	XMMATRIX mSpin = XMMatrixRotationZ(-t);
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
	XMMATRIX mTranslate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	m_smallCubeWorld = mScale * mSpin * mTranslate * mOrbit;
}

//-----------------------------------------------------------------------------------------------
void Game::Render() const
{
	// this might be needed to go first once we do more complicated things
	//m_deviceImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	float color[4] = { 210.f/255.f, 74.f / 255.f, 97.f / 255.f, 1.0f };
	
	m_deviceImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_deviceImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// set shaders
	m_deviceImmediateContext->VSSetShader(m_vertexShader, nullptr, 0);
	m_deviceImmediateContext->PSSetShader(m_pixelShader, nullptr, 0);

	// draw big cube
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_bigCubeWorld);
	cb.mView = XMMatrixTranspose(m_view);
	cb.mProjection = XMMatrixTranspose(m_projection);
	m_deviceImmediateContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb, 0, 0);

	m_deviceImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	m_deviceImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list
	
															
	// second cube
	// seems we are just using the same mesh as the cube, just sticking it in another place
	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_smallCubeWorld);
	cb2.mView = XMMatrixTranspose(m_view);
	cb2.mProjection = XMMatrixTranspose(m_projection);
	
	m_deviceImmediateContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb2, 0, 0);

	m_deviceImmediateContext->DrawIndexed(36, 0, 0);
															
															
	//uint stride = sizeof(Vertex);
	//uint offset = 0;
	//m_deviceImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//
	//m_deviceImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set indices as well


	m_pSwapChain->Present(0, 0);
}



//-----------------------------------------------------------------------------------------------
// doesn't do any deleting
bool Game::LoadCompiledShaderFromFile(const std::string& path, int& sizeOut, char*& out)
{
	// I would make this into a function thats like "LoadFileIntoBuffer"
	// and add some error checking
	
	FILE *fl;
	fopen_s(&fl, path.c_str(), "rb");
	
	fseek(fl, 0, SEEK_END);
	long len = ftell(fl);
	
	out = (char*)malloc(len);
	
	fseek(fl, 0, SEEK_SET);
	fread(out, 1, len, fl);
	fclose(fl);

	sizeOut = len;

	return true;
}

