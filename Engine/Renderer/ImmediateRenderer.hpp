#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Core/General/EngineCommon.hpp"

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

	void Sphere(const Vector3& position, float radius = 1.f, Rgba theColor = Rgba::WHITE, uint wedges = 8, uint slices = 8);


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