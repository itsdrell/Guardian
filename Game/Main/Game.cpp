#include "Game.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>

#include <directxmath.h>
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/ThirdParty/stbi/stb_image.h"

static float depthPos = -4.5f;

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
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_vertexShader->Release();
	if (m_vertexLayout) m_vertexLayout->Release();
	if (m_vertexBuffer) m_vertexBuffer->Release();
}

//-----------------------------------------------------------------------------------------------
void Game::StartUp()
{
	HRESULT hr = S_OK;
	Window* theWindow = Window::GetInstance();
	Renderer* r = Renderer::GetInstance();

	//-----------------------------------------------------------------------------------------------
	// Create pixel shader
	int pixelShaderlength = 0;
	char* pixelShaderBuffer = nullptr;
	
	bool result = LoadCompiledShaderFromFile("Data\\Shaders\\Compiled\\TestPixelShader.cso", pixelShaderlength, pixelShaderBuffer);
	if(result == false)
	{
		return;
	}
	
	hr = r->m_deviceInterface->CreatePixelShader(pixelShaderBuffer, pixelShaderlength, nullptr, &m_pixelShader);

	//-----------------------------------------------------------------------------------------------
	// create vertex shader
	int vertexShaderLength = 0;
	char* vertexShaderBuffer = nullptr;
	result = LoadCompiledShaderFromFile("Data\\Shaders\\Compiled\\TestVertShader.cso", vertexShaderLength, vertexShaderBuffer);

	if (result == false)
	{
		return;
	}
	
	hr = r->m_deviceInterface->CreateVertexShader(vertexShaderBuffer, vertexShaderLength, nullptr, &m_vertexShader);

	// input layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE_COORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = r->m_deviceInterface->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBuffer, vertexShaderLength, &m_vertexLayout);
	r->m_deviceImmediateContext->IASetInputLayout(m_vertexLayout);

	//-----------------------------------------------------------------------------------------------
	// set the shaders as active
	r->m_deviceImmediateContext->VSSetShader(m_vertexShader, nullptr, 0);
	r->m_deviceImmediateContext->PSSetShader(m_pixelShader, nullptr, 0);
	
	free(pixelShaderBuffer);
	free(vertexShaderBuffer);

	//-----------------------------------------------------------------------------------------------
	// Create the vertex buffer
	//Vertex vertices[] =
	//{
	//	Vertex(Vector3(-1.0f,  1.0f, -1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(1.0f,  1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(1.0f,  1.0f,  1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(-1.0f,  1.0f,  1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(1.0f, -1.0f,  1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//	Vertex(Vector3(-1.0f, -1.0f,  1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.f, 0.f)),
	//};

	Vertex vertices[] =
	{
		Vertex(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
												
		Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),

		Vertex(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
		Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),

		Vertex(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),

		Vertex(Vector3(-1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
		Vertex(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),

		Vertex(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
		Vertex(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
		Vertex(Vector3(1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
	};

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;

	hr = r->m_deviceInterface->CreateBuffer(&bufferDescription, &InitData, &m_vertexBuffer);

	// I feel like this part should happen in rendering every frame, but doing it once we 
	// create the buffer could be a nice sanity check (or waste)
	// should def be a function we call before drawing
	uint stride = sizeof(Vertex);
	uint offset = 0;
	r->m_deviceImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//-----------------------------------------------------------------------------------------------
	// Index buff creation
	// Create index buffer
	uint16 indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = sizeof(uint16) * 36; // 36 vertices needed for 12 triangles in a triangle list
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	InitData.pSysMem = indices;
	
	hr = r->m_deviceInterface->CreateBuffer(&indexBufferDescription, &InitData, &m_indexBuffer);
	r->m_deviceImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//-----------------------------------------------------------------------------------------------
	r->m_deviceImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//-----------------------------------------------------------------------------------------------
	// Constant buffer
	D3D11_BUFFER_DESC constantBufferDescription = {};
	constantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDescription.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = 0;

	hr = r->m_deviceInterface->CreateBuffer(&constantBufferDescription, nullptr, &m_constantBuffer);

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


	//-----------------------------------------------------------------------------------------------
	// Texture stuff :l 
	int textureDimensionX = 0;
	int textureDimensionY = 0;
	int numberOfComponents = 0;
	int numberOfComponentsRequested = 0;

	// Get the buffer using stbi
	// Load (and decompress) the image RGB(A) bytes from a file on disk, and create an OpenGL texture instance from it
	unsigned char* imageData = stbi_load("Data/Images/Test_StbiAndDirectX.png", &textureDimensionX, &textureDimensionY, &numberOfComponents, numberOfComponentsRequested);

	// Create the texture resource (https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-create)
	// https://www.rastertek.com/dx11s2tut05.html
	// This is creating a default texture we are going to fill with data
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = textureDimensionX;
	textureDesc.Height = textureDimensionY;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 0; 
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = r->m_deviceInterface->CreateTexture2D(&textureDesc, NULL, &m_testTexture);
	r->m_deviceImmediateContext->UpdateSubresource(m_testTexture, 0, NULL, imageData, (textureDesc.Width * 4) * sizeof(unsigned char), 0);

	// Create the Shader Resource View for the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = r->m_deviceInterface->CreateShaderResourceView(m_testTexture, &shaderResourceViewDesc, &m_testTextureView);
	r->m_deviceImmediateContext->GenerateMips(m_testTextureView);

	// Create the sampler description
	D3D11_SAMPLER_DESC sampleDescription = {};
	sampleDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDescription.MinLOD = 0;
	sampleDescription.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler
	hr = r->m_deviceInterface->CreateSamplerState(&sampleDescription, &m_testTextureSampler);

	// Bind both sampler and texture to shader
	r->m_deviceImmediateContext->PSSetShaderResources(0, 1, &m_testTextureView);
	r->m_deviceImmediateContext->PSSetSamplers(0, 1, &m_testTextureSampler);

	stbi_image_free(imageData);
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

	depthPos += (.005f);

	// 2nd Cube:  Rotate around origin
	XMMATRIX mSpin = XMMatrixRotationZ(-t); //t
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f); //-t * 2.0f
	XMMATRIX mTranslate = XMMatrixTranslation(0.0f, 1.0f, depthPos);
	XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	m_smallCubeWorld = mScale * mSpin * mTranslate * mOrbit;
}

//-----------------------------------------------------------------------------------------------
void Game::Render() const
{
	Renderer* r = Renderer::GetInstance();
	
	// set shaders
	r->m_deviceImmediateContext->VSSetShader(m_vertexShader, nullptr, 0);
	r->m_deviceImmediateContext->PSSetShader(m_pixelShader, nullptr, 0);

	// draw big cube
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_bigCubeWorld);
	cb.mView = XMMatrixTranspose(m_view);
	cb.mProjection = XMMatrixTranspose(m_projection);
	r->m_deviceImmediateContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb, 0, 0);

	r->m_deviceImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	r->m_deviceImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list
	
															
	// second cube
	// seems we are just using the same mesh as the cube, just sticking it in another place
	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_smallCubeWorld);
	cb2.mView = XMMatrixTranspose(m_view);
	cb2.mProjection = XMMatrixTranspose(m_projection);
	
	r->m_deviceImmediateContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb2, 0, 0);

	//m_deviceImmediateContext->DrawIndexed(36, 0, 0);
															
															
	//uint stride = sizeof(Vertex);
	//uint offset = 0;
	//m_deviceImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//
	//m_deviceImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set indices as well
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

