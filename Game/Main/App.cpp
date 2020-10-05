#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "App.hpp"
#include "Game/Main/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/EngineCommon.hpp"

//===============================================================================================

App* g_theApp = nullptr;

//===============================================================================================
App::App()
{
	EngineStartUp();
	
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
	Renderer::GetInstance()->BeginFrame();
	Update();
	Render();
	Renderer::GetInstance()->EndFrame();

	Sleep(1);
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
