#include "ImmediateRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"

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

