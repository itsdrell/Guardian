#include "App.hpp"
#include "Game/Main/Game.hpp"
#include "Engine/Core/General/Camera.hpp"

//===============================================================================================

App* g_theApp = nullptr;

//===============================================================================================
App::App()
{
	new Game();
}

//-----------------------------------------------------------------------------------------------
App::~App()
{
	delete g_theGame;
	g_theGame = nullptr;
}

//-----------------------------------------------------------------------------------------------
void App::Startup()
{
}

//-----------------------------------------------------------------------------------------------
void App::RunFrame()
{
	Update();
	Render();
}

//-----------------------------------------------------------------------------------------------
void App::Update()
{
	// nothing for now
	g_theGame->Update(.03f);
}

//-----------------------------------------------------------------------------------------------
void App::Render() const
{
	g_theGame->Render();
}

//-----------------------------------------------------------------------------------------------
void App::RenderTriangle() const
{
	//Renderer* r = Renderer::GetInstance();
	//
	//// The code we want
	//r->m_defaultCamera->SetProjectionOrthoByAspect(10.f);
	//
	//r->SetCamera(r->m_defaultCamera);
	//r->SetShader(r->m_defaultShader);
	//r->SetCurrentTexture();
	//
	//Vertex3D_PCU triangle[3] = 
	//{
	//	Vertex3D_PCU(Vector3(1.f, 0.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
	//	Vertex3D_PCU(Vector3(0.f, 1.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
	//	Vertex3D_PCU(Vector3(-1.f, 0.f, 1.f), Rgba(255,0,0), Vector2(0.f, 1.f)),
	//};
	//
	//r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, triangle, 3); //DrawVertexArray
	//r->SetCamera(r->m_defaultCamera);

}
