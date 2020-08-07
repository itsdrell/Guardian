#include "EngineCommon.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

//===============================================================================================
void EngineStartUp()
{
	Renderer* renderer = new Renderer();
	renderer->RenderStartup(Window::GetInstance()->GetHandle()); // call the static variable
	renderer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	delete Renderer::GetInstance();
}
