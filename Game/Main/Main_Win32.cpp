#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places
#include <math.h>
#include <cassert>
#include <crtdbg.h>

#include "App.hpp"
#include "Engine\Core\Platform\Window.hpp"
#pragma comment( lib, "opengl32" )	// Link in the OpenGL32.lib static library
#pragma comment(lib,"d3d11.lib")

//-----------------------------------------------------------------------------------------------
constexpr float CLIENT_ASPECT = 1.77f; // We are requesting a 1:1 aspect (square) window area

							
HWND g_hWnd = nullptr;							// ...becomes WindowContext::m_windowHandle
HDC g_displayDeviceContext = nullptr;			// ...becomes WindowContext::m_displayContext
HGLRC g_openGLRenderingContext = nullptr;		// ...becomes RendererOpenGL::m_glRenderingContext


//-----------------------------------------------------------------------------------------------
bool AppMessageHandler(unsigned int msg, size_t wparam, size_t lparam)
{
	switch( msg )
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
	case WM_CLOSE:		
	{
		g_theApp->m_isQuitting = true;
		return false; // "Consumes" this message (tells Windows "okay, we handled it")
	}

	// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
	case WM_KEYDOWN:
	{
		// keeping the default close in main
		if( wparam == VK_ESCAPE )
		{
			g_theApp->m_isQuitting = true;
			return false; // "Consumes" this message (tells Windows "okay, we handled it")
		}

		break;
	}

	// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
	case WM_KEYUP:
	{
		break;
	}

	}
	return true;
}

//-----------------------------------------------------------------------------------------------
void CreateAppWindow( HINSTANCE applicationInstanceHandle, float clientAspect )
{
	Window* newWindow = new Window("Guardian", clientAspect);
	newWindow->RegisterHandler( AppMessageHandler );
}

//-----------------------------------------------------------------------------------------------
// Processes all Windows messages (WM_xxx) for this app that have queued up since last frame.
// For each message in the queue, our WindowsMessageHandlingProcedure (or "WinProc") function
//	is called, telling us what happened (key up/down, minimized/restored, gained/lost focus, etc.)
//
void RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage ); // This tells Windows to call our "WindowsMessageHandlingProcedure" function
	}
}

//-----------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void RunFrame()
{
	RunMessagePump();
	g_theApp->RunFrame();
}


//-----------------------------------------------------------------------------------------------
void Initialize( HINSTANCE applicationInstanceHandle )
{
	CreateAppWindow( applicationInstanceHandle, CLIENT_ASPECT );
	g_theApp = new App();	
	g_theApp->Startup();
}


//-----------------------------------------------------------------------------------------------
void Shutdown()
{
	// Destroy the global App instance
	delete g_theApp;			
	g_theApp = nullptr;
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	Initialize( applicationInstanceHandle );

	// Program main loop; keep running frames until it's time to quit
	while( !g_theApp->m_isQuitting ) 
	{
		RunFrame();
	}

	Shutdown();
	return 0;
}


