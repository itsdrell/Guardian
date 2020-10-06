#include "EngineCommon.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ImmediateRenderer.hpp"

//===============================================================================================
void EngineStartUp()
{
	Renderer* renderer = new Renderer();
	renderer->Startup();
	g_draw = new ImmediateRenderer(renderer);
	renderer = nullptr;

}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	delete Renderer::GetInstance();
}
