#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>

#include <string>
#include <vector>

#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

using namespace DirectX;

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct Vertex
{
	Vertex(const Vector3& thePos, const Vector4& theColor, const Vector2& theUvs)
		: pos(thePos)
		, color(theColor) 
		, uv(theUvs) {}

	Vector3 pos;
	Vector4	color; // normalized
	Vector2 uv;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

//====================================================================================
// Classes
//====================================================================================
class Game
{
public:
	Game();
	~Game();

public: 
	void StartUp();
	void Update(float ds);
	void Render() const;

private:


	ID3D11InputLayout*		m_vertexLayout = nullptr;
	ID3D11Buffer*			m_vertexBuffer = nullptr;
	ID3D11Buffer*			m_indexBuffer = nullptr;

	ID3D11BlendState*		m_blendState = nullptr;

	// constant buffer for transformations
	ID3D11Buffer*			m_constantBuffer = nullptr;
	XMMATRIX				m_bigCubeWorld;
	XMMATRIX				m_smallCubeWorld;

	XMMATRIX				m_view;
	XMMATRIX				m_projection;				
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern Game* g_theGame;

//====================================================================================
// Written by Zachary Bracken : [8/2/2020]
//====================================================================================