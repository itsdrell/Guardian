#include "Game.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/General/Camera.hpp"

static float depthPos = -4.5f;
static float totalTime = 0.f;

//===============================================================================================
Game* g_theGame = nullptr;
//===============================================================================================
std::vector<VertexMaster> vertices =
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

std::vector<uint16> indices =
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
	delete m_camera;
	m_camera = nullptr;
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
	
	r->SetActiveShader(r->m_testShader);
	r->SetActiveTexture(0, r->m_testTexture);
	r->SetCamera(m_camera);

	r->SetModel(m_bigCubeModel);
	r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint) vertices.size(), vertices.data(), (uint) indices.size(), indices.data());
														
	// second cube
	r->SetModel(m_smallCubeModel);
	r->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint) vertices.size(), vertices.data(), (uint) indices.size(), indices.data());
}
