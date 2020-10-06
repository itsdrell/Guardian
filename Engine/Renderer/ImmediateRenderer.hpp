#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Core/General/Rgba.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Renderer;
class Vector3;
class Texture;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct Rect
{
	Rect(Vector2 start, Vector2 end);
	~Rect() {};

	const static Rect ZERO_TO_ONE;

	Vector2 p0;
	Vector2 p1;
};

//====================================================================================
// Classes
//====================================================================================
class ImmediateRenderer
{
public:
	ImmediateRenderer(Renderer* theRenderer);

public:
	void Cube(const Vector3& center, const Vector3& dimensions, Texture* textureToUse = nullptr, const Rgba &color = Rgba::WHITE,
		Rect uv_top = Rect::ZERO_TO_ONE, Rect uv_side = Rect::ZERO_TO_ONE, Rect uv_bottom = Rect::ZERO_TO_ONE);

private:
	Renderer*	m_renderer = nullptr;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern ImmediateRenderer* g_draw;

//====================================================================================
// Written by Zachary Bracken : [10/5/2020]
//====================================================================================