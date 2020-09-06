#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11Buffer;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum eBufferType
{
	BUFFER_TYPE_VERTEX,
	BUFFER_TYPE_INDEX,
	BUFFER_TYPE_CONSTANT,
	NUM_BUFFER_TYPES
};

//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class RenderBuffer
{
public:
	RenderBuffer(eBufferType type, uint size, uint stride, void* bufferData);
	
	~RenderBuffer();

private:
	void CreateBuffer(eBufferType type, uint size, uint stride, void* bufferData);

public:
	ID3D11Buffer*		m_buffer = nullptr;
	uint				m_size = 0U;
	uint				m_stride = 0U;

	// idk why we would need this atm but I will need it for binding so setting it to the default
	uint				m_offset = 0U;
	
	eBufferType			m_type = NUM_BUFFER_TYPES;
};

//===============================================================================================
// Just to make the variables cleaner
// Possibly overkill but its nice to new off a Vertex buffer vs a Constant buffer and relying on 
// the name to tell you what it is
//===============================================================================================
class VertexBuffer : public RenderBuffer
{
public:
	VertexBuffer(uint size, uint stride, void* bufferData)
		: RenderBuffer(BUFFER_TYPE_VERTEX, size,stride, bufferData) {}
};

//-----------------------------------------------------------------------------------------------
class IndexBuffer : public RenderBuffer
{
public:
	IndexBuffer(uint size, uint stride, void* bufferData)
		: RenderBuffer(BUFFER_TYPE_INDEX, size, stride, bufferData) {}
};

//-----------------------------------------------------------------------------------------------
class ConstantBuffer : public RenderBuffer
{
public:
	ConstantBuffer(uint size, uint stride, void* bufferData = nullptr)
		: RenderBuffer(BUFFER_TYPE_CONSTANT, size, stride, bufferData) {}
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/6/2020]
//====================================================================================