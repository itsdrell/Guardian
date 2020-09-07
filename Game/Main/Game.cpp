#include "Game.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Renderer/Pipeline/ShaderProgram.hpp"
#include "Engine/Renderer/Pipeline/RenderBuffers.hpp"
#include "Engine/Renderer/RenderTypes.hpp"

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
VertexMaster vertices[] =
{
	VertexMaster(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, 1.0f),			Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
	VertexMaster(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),

	VertexMaster(Vector3(-1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
	VertexMaster(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
	VertexMaster(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
	VertexMaster(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),

	VertexMaster(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
	VertexMaster(Vector3(-1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
	VertexMaster(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
	VertexMaster(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),

	VertexMaster(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, 1.0f),			Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),

	VertexMaster(Vector3(-1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, -1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
	VertexMaster(Vector3(-1.0f, 1.0f, -1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),

	VertexMaster(Vector3(-1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, -1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 1.0f)),
	VertexMaster(Vector3(1.0f, 1.0f, 1.0f),			Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(0.0f, 0.0f)),
	VertexMaster(Vector3(-1.0f, 1.0f, 1.0f),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) , Vector2(1.0f, 0.0f)),
};

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

//===============================================================================================
Game::Game()
{
	g_theGame = this;

	StartUp();
}

//-----------------------------------------------------------------------------------------------
Game::~Game()
{
	delete m_constantBuffer;
	m_constantBuffer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Game::StartUp()
{
	Window* theWindow = Window::GetInstance();

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


	//-----------------------------------------------------------------------------------------------
	// Constant buffer
	// giving it a default constant buffer
	m_constantBuffer = new ConstantBuffer(1, sizeof(TestConstantBuffer), &TestConstantBuffer());

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
	
	r->SetActiveShader(r->m_testShader);
	r->SetActiveTexture(0, r->m_testTexture);

	// draw big cube
	TestConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_bigCubeWorld);
	cb.mView = XMMatrixTranspose(m_view);
	cb.mProjection = XMMatrixTranspose(m_projection);

	r->UpdateConstantBuffer(m_constantBuffer, &cb);
	r->SetConstantBuffer(0, m_constantBuffer);

	r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, ARRAYSIZE(vertices), vertices, ARRAYSIZE(indices), indices);
														
	// second cube
	// seems we are just using the same mesh as the cube, just sticking it in another place
	TestConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_smallCubeWorld);
	cb2.mView = XMMatrixTranspose(m_view);
	cb2.mProjection = XMMatrixTranspose(m_projection);
	
	r->UpdateConstantBuffer(m_constantBuffer, &cb2);

	r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, ARRAYSIZE(vertices), vertices, ARRAYSIZE(indices), indices);
}
