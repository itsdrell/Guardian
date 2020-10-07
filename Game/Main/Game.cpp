#include "Game.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/ImmediateRenderer.hpp"

static float depthPos = -4.5f;
static float totalTime = 0.f;

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
	delete m_camera;
	m_camera = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Game::StartUp()
{
	Window* theWindow = Window::GetInstance();

	Vector3 eye = Vector3(0.f, 1.0f, -5.0f);
	Vector3 targetPos = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.f);

	Matrix44 view = Matrix44::LookAt(eye, targetPos, up);
	Matrix44 projection = Matrix44::MakePerspectiveProjection(45, theWindow->GetAspect(), .01f, 100.f);

	m_camera = new Camera(Matrix44(), view, projection);
}

//-----------------------------------------------------------------------------------------------
void Game::Update(float ds)
{
	totalTime += ds;
	
	m_bigCubeModel = Matrix44::MakeRotationDegreesAroundY(totalTime * 10);

	depthPos += (.005f);

	// 2nd Cube:  Rotate around origin
	Matrix44 spin = Matrix44::MakeRotationDegreesAroundZ(-totalTime);
	Matrix44 orbit = Matrix44::MakeRotationDegreesAroundY(-totalTime * 2.0f);
	Matrix44 translate = Matrix44::MakeTranslation3D(Vector3(0.0f, 1.0f, depthPos));
	m_smallCubeModel = Matrix44();
	m_smallCubeModel.Append(orbit);
	m_smallCubeModel.Append(translate);
	m_smallCubeModel.Append(spin);
}

//-----------------------------------------------------------------------------------------------
void Game::Render() const
{
	Renderer* r = Renderer::GetInstance();
	
	r->SetCamera(m_camera);

	r->SetActiveShader(r->m_testShader);
	r->SetModel(m_bigCubeModel);
	g_draw->Cube(Vector3(0.f, 0.f, 0.f), Vector3(2.f, 2.f, 2.f), r->m_testTexture);

	r->SetActiveShader(r->m_wireFrameShader);
	r->SetModel(Matrix44());
	g_draw->Cube(Vector3(-3.f, 0.f, -1.f), Vector3(1.f, 1.f, 1.f), r->m_defaultTexture, Rgba::WHITE);
	
	Matrix44 sphereModel = Matrix44::MakeTranslation3D(Vector3(3.f, 0.f, -1.f));
	sphereModel.Append(m_bigCubeModel);
	r->SetModel(sphereModel);
	g_draw->Sphere(Vector3(0.f, 0.f, 0.f), 1.f, Rgba::BLUE);

	// second cube
	r->SetActiveShader(r->m_testShader);
	r->SetModel(m_smallCubeModel);
	//g_draw->Cube(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f), r->m_defaultTexture, Rgba::YELLOW);

	r->SetModel(Matrix44());
	g_draw->Point3D(Vector3(-2.f, 2.f, -1.f), Rgba::BLUE);
	g_draw->Basis(m_bigCubeModel, 2.f, 1.f);
}
