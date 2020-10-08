#include "ImmediateRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//===============================================================================================
ImmediateRenderer* g_draw = nullptr;

//===============================================================================================
const Rect Rect::ZERO_TO_ONE = Rect(Vector2(0.f,0.f), Vector2(1.f, 1.f));

Rect::Rect(Vector2 start, Vector2 end)
	: p0(start)
	, p1(end) {}

//===============================================================================================
ImmediateRenderer::ImmediateRenderer(Renderer* theRenderer)
{
	m_renderer = theRenderer;
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Point3D(const Vector3& position, const Rgba& color /*= Rgba::WHITE*/, float scale)
{
	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);

	// this is how you get like a pixel point but its so small
	//std::vector<VertexMaster> vertices = { VertexMaster(position, color.GetAsNormalizedVector4(), Vector2(0.f, 1.f)) };
	//m_renderer->DrawMeshImmediate(PRIMITIVE_POINTS, 1, vertices.data(), 0, nullptr);

	// this draws a star at a point
	Vector3 north = position + (Vector3::UP * scale);
	Vector3 south = position - (Vector3::UP * scale);
	Vector3 east = position + (Vector3::RIGHT * scale);
	Vector3 west = position - (Vector3::RIGHT * scale);

	Vector3 forward = position + (Vector3::FORWARD * scale);
	Vector3 backward = position - (Vector3::FORWARD * scale);

	Vector3 ne = position + ((Vector3::UP + Vector3::RIGHT) * scale);
	Vector3 nw = position + ((Vector3::UP + Vector3::LEFT) * scale);
	Vector3 se = position + ((Vector3::DOWN + Vector3::RIGHT) * scale);
	Vector3 sw = position + ((Vector3::DOWN + Vector3::LEFT) * scale);

	Line3D(north, south, 1.f, color);
	Line3D(east, west, 1.f, color);
	Line3D(ne, sw, 1.f, color);
	Line3D(nw, se, 1.f, color);
	Line3D(forward, backward, 1.f, color);
}

//-----------------------------------------------------------------------------------------------
// Note: This might need to not use Primitive lines and use triangles instead
void ImmediateRenderer::Line3D(const Vector3& start, const Vector3& end, float thickness, const Rgba& color /*= Rgba::WHITE*/)
{
	UNUSED(thickness); // we might use it later but the lines look fine as is
	
	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);

	std::vector<VertexMaster> vertices = 
	{ 
		VertexMaster(start, color.GetAsNormalizedVector4(), Vector2(0.f, 0.f)),
		VertexMaster(end, color.GetAsNormalizedVector4(), Vector2(0.f, 0.f))
	};

	m_renderer->DrawMeshImmediate(PRIMITIVE_LINES, 2, vertices.data(), 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Basis(const Matrix44& basis, float lengthOfLine, float lineThickness)
{
	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);

	Vector3 position = basis.GetPosition();

	Vector3 x = position + (basis.GetRight() * lengthOfLine);
	Vector3 y = position + (basis.GetUp() * lengthOfLine);
	Vector3 z = position + (basis.GetForward() * lengthOfLine);

	Line3D(position, x, lineThickness, Rgba::RED);
	Line3D(position, y, lineThickness, Rgba::YELLOW);
	Line3D(position, z, lineThickness, Rgba::BLUE);
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Quad3D(const Vector3& pos, const Vector2& dimensions, const Rgba& color /*= Rgba::WHITE*/, const Texture* textureToUse /*= nullptr*/,
	const Vector3& right /*= Vector3::RIGHT*/, const Vector3& up /*= Vector3::UP*/)
{
	if (textureToUse == nullptr)
		textureToUse = m_renderer->m_testTexture;

	m_renderer->SetActiveTexture(0, textureToUse);
	
	Vector3 topRight = pos + (right * dimensions.x) + (up * dimensions.y);
	Vector3 topLeft = pos + (-right * dimensions.x) + (up * dimensions.y);
	Vector3 bottomRight = pos + (right * dimensions.x) + (-up * dimensions.y);
	Vector3 bottomLeft = pos + (-right * dimensions.x) + (-up * dimensions.y);

	Vector4 colorAsVector = color.GetAsNormalizedVector4();

	std::vector<VertexMaster> vertices =
	{
		// top
		VertexMaster(bottomLeft,	colorAsVector, Vector2(0.f, 1.f)),
		VertexMaster(bottomRight,	colorAsVector, Vector2(1.f, 1.f)),
		VertexMaster(topRight,		colorAsVector, Vector2(1.f, 0.f)),
		VertexMaster(topLeft,		colorAsVector, Vector2(0.f, 0.f)),
	};

	std::vector<uint16> indices =
	{
		3,1,0,
		2,1,3
	};

	m_renderer->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint)vertices.size(), vertices.data(), (uint)indices.size(), indices.data());
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Cube(const Vector3& center, const Vector3& dimensions, Texture* textureToUse /*= nullptr*/, const Rgba &color /*= Rgba::WHITE*/, 
	Rect uv_top /*= Rect::ZERO_TO_ONE*/, Rect uv_side /*= Rect::ZERO_TO_ONE*/, Rect uv_bottom /*= Rect::ZERO_TO_ONE*/)
{
	if (textureToUse == nullptr)
		textureToUse = m_renderer->m_testTexture;

	m_renderer->SetActiveTexture(0, textureToUse);

	Vector4 colorAsVector = color.GetAsNormalizedVector4();
	Vector3 halfDimensions = Vector3(dimensions.x * .5f, dimensions.y * .5f, dimensions.z * .5f);

	std::vector<VertexMaster> vertices =
	{
		// top
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_top.p1.x, uv_top.p0.y)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_top.p0)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_top.p0.x, uv_top.p1.y)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_top.p1)),

		// bottom										  
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_bottom.p0)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_bottom.p1.x, uv_bottom.p0.y)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_bottom.p1)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_bottom.p0.x, uv_bottom.p1.y)),

		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p0.x, uv_side.p1.y)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p1)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p1.x, uv_side.p0.y)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p0)),

		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p1)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p0.x, uv_side.p1.y)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p0)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p1.x, uv_side.p0.y)),

		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p0.x, uv_side.p1.y)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p1)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p1.x, uv_side.p0.y)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z - halfDimensions.z),	colorAsVector, Vector2(uv_side.p0)),

		VertexMaster(Vector3(center.x - halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p1)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y - halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p0.x, uv_side.p1.y)),
		VertexMaster(Vector3(center.x + halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p0)),
		VertexMaster(Vector3(center.x - halfDimensions.x, center.y + halfDimensions.y, center.z + halfDimensions.z),	colorAsVector, Vector2(uv_side.p1.x, uv_side.p0.y)),
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

	m_renderer->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint) vertices.size(), vertices.data(), (uint) indices.size(), indices.data());
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Sphere(const Vector3& position, float radius /*= 1.f*/, Rgba theColor /*= Rgba::WHITE*/, uint wedges /*= 8*/, uint slices /*= 8*/)
{
	std::vector<VertexMaster> vertices;
	std::vector<uint16> indicies;

	Vector4 colorAsVector4 = theColor.GetAsNormalizedVector4();
	
	for (uint slice_idx = 0; slice_idx <= slices; ++slice_idx)
	{

		float v = (float)slice_idx / ((float)slices);
		float azimuth = RangeMapFloat(v, 0, 1, -90, 90);

		for (uint wedge_idx = 0; wedge_idx <= wedges; ++wedge_idx)
		{

			float u = (float)wedge_idx / ((float)wedges);
			float rot = 360 * u;

			Vector2 uvs = Vector2(u, v);
			Vector3 pos = position + PolarToCartesian(radius, rot, azimuth);

			vertices.push_back(VertexMaster(pos, colorAsVector4, uvs));
		}
	}

	for (uint slice_idx = 0; slice_idx < slices; ++slice_idx) //y
	{
		for (uint wedge_idx = 0; wedge_idx < wedges; ++wedge_idx) //x
		{

			uint16 bl_idx = (uint16)(((wedges + 1) * slice_idx) + wedge_idx);
			uint16 tl_idx = (uint16) (bl_idx + wedges + 1); //bl_idx + wedges; 
			uint16 br_idx = bl_idx + 1;
			uint16 tr_idx = tl_idx + 1;

			indicies.emplace_back(bl_idx);
			indicies.emplace_back(br_idx);
			indicies.emplace_back(tr_idx);

			indicies.emplace_back(bl_idx);
			indicies.emplace_back(tr_idx);
			indicies.emplace_back(tl_idx);
		}
	}

	m_renderer->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint)vertices.size(), vertices.data(), (uint)indicies.size(), indicies.data());
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Line2D(const Vector2& start, const Vector2& end, const Rgba& color /*= Rgba(0, 0, 0, 255)*/)
{
	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);

	std::vector<VertexMaster> vertices =
	{
		VertexMaster(Vector3(start.x, start.y, 0.f), color.GetAsNormalizedVector4(), Vector2(0.f, 0.f)),
		VertexMaster(Vector3(end.x, end.y, 0.f), color.GetAsNormalizedVector4(), Vector2(0.f, 0.f))
	};

	m_renderer->DrawMeshImmediate(PRIMITIVE_LINES, 2, vertices.data(), 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Circle2D(const Vector2& center, float radius, const Rgba& color /*= Rgba(255, 255, 255, 255)*/, int numberOfEdges /*= 60*/)
{
	float distanceInDegrees = 360.f / ((float)numberOfEdges);
	float degrees = 0.f;

	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);
	Vector4 colorAsVector4 = color.GetAsNormalizedVector4();

	std::vector<VertexMaster> vertices;

	float previousPointX = center.x + (radius * (CosDegrees(0)));
	float previousPointY = center.y + (radius * (SinDegrees(0)));
	for (int i = 1; i < numberOfEdges + 1; i++)
	{
		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float nextPointX = center.x + (radius * (CosDegrees(degrees)));
		float nextPointY = center.y + (radius * (SinDegrees(degrees)));

		// counter clockwise 
		vertices.push_back(VertexMaster(Vector3(center.x,center.y, 0.f),				colorAsVector4, Vector2(0.f, 0.f)));
		vertices.push_back(VertexMaster(Vector3(nextPointX, nextPointY, 0.f),			colorAsVector4, Vector2(0.f, 0.f)));
		vertices.push_back(VertexMaster(Vector3(previousPointX, previousPointY, 0.f),	colorAsVector4, Vector2(0.f, 0.f)));

		previousPointX = nextPointX;
		previousPointY = nextPointY;
	}

	m_renderer->DrawMeshImmediate(PRIMITIVE_TRIANGLES, numberOfEdges * 3, vertices.data(), 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Circle2DOutline(const Vector2& center, float radius, const Rgba& color /*= Rgba(255, 255, 255, 255)*/, int numberOfEdges /*= 60*/)
{
	float distanceInDegrees = 360.f / ((float)numberOfEdges);
	float degrees = 0.f;

	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);
	Vector4 colorAsVector4 = color.GetAsNormalizedVector4();

	std::vector<VertexMaster> vertices;

	float previousPointX = center.x + (radius * (CosDegrees(0)));
	float previousPointY = center.y + (radius * (SinDegrees(0)));
	for (int i = 1; i < numberOfEdges + 1; i++)
	{
		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float nextPointX = center.x + (radius * (CosDegrees(degrees)));
		float nextPointY = center.y + (radius * (SinDegrees(degrees)));

		vertices.push_back(VertexMaster(Vector3(nextPointX, nextPointY, 0.f), colorAsVector4, Vector2(0.f, 0.f)));
		vertices.push_back(VertexMaster(Vector3(previousPointX, previousPointY, 0.f), colorAsVector4, Vector2(0.f, 0.f)));

		previousPointX = nextPointX;
		previousPointY = nextPointY;
	}

	m_renderer->DrawMeshImmediate(PRIMITIVE_LINES, numberOfEdges * 2, vertices.data(), 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Rect2D(const AABB2& bounds, const Rgba& tint /*= Rgba::WHITE*/, const Texture* texture /*= nullptr*/, 
	const Vector2& texCoordsAtMins /*= Vector2::ZERO*/, const Vector2& texCoordsAtMaxs /*= Vector2::ONE*/)
{
	if (texture == nullptr)
		texture = m_renderer->m_defaultTexture;

	m_renderer->SetActiveTexture(0, texture);
	
	Vector4 colorAsVector = tint.GetAsNormalizedVector4();

	std::vector<VertexMaster> vertices =
	{
		VertexMaster(Vector3(bounds.mins.x, bounds.mins.y, 0.f),	colorAsVector, Vector2(texCoordsAtMins.x, texCoordsAtMaxs.y)),
		VertexMaster(Vector3(bounds.maxs.x, bounds.mins.y, 0.f),	colorAsVector, texCoordsAtMaxs),
		VertexMaster(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f),	colorAsVector, Vector2(texCoordsAtMaxs.x, texCoordsAtMins.y)),
		VertexMaster(Vector3(bounds.mins.x, bounds.maxs.y, 0.f),	colorAsVector, texCoordsAtMins),
	};

	std::vector<uint16> indices =
	{
		3,1,0,
		2,1,3
	};

	m_renderer->DrawMeshImmediate(PRIMITIVE_TRIANGLES, (uint)vertices.size(), vertices.data(), (uint)indices.size(), indices.data());
}

//-----------------------------------------------------------------------------------------------
void ImmediateRenderer::Rect2DOutline(const AABB2& bounds, const Rgba& tint /*= Rgba::WHITE*/)
{
	m_renderer->SetActiveTexture(0, m_renderer->m_defaultTexture);

	Vector4 colorAsVector = tint.GetAsNormalizedVector4();

	std::vector<VertexMaster> vertices =
	{
		VertexMaster(Vector3(bounds.mins.x, bounds.mins.y, 0.f),	colorAsVector, Vector2::ZERO),
		VertexMaster(Vector3(bounds.maxs.x, bounds.mins.y, 0.f),	colorAsVector, Vector2::ZERO),
		VertexMaster(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f),	colorAsVector, Vector2::ZERO),
		VertexMaster(Vector3(bounds.mins.x, bounds.maxs.y, 0.f),	colorAsVector, Vector2::ZERO),
	};

	std::vector<uint16> indices =
	{
		0,1,
		1,2,
		2,3,
		3,0
	};

	m_renderer->DrawMeshImmediate(PRIMITIVE_LINES, (uint)vertices.size(), vertices.data(), (uint)indices.size(), indices.data());
}

