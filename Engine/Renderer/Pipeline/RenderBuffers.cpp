#include "RenderBuffers.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <map>
#include <d3d11_1.h>

//===============================================================================================
static std::map<eBufferType,uint> bindFlagConverter = 
{
	{ BUFFER_TYPE_VERTEX,	D3D11_BIND_VERTEX_BUFFER },
	{ BUFFER_TYPE_INDEX,	D3D11_BIND_INDEX_BUFFER },
	{ BUFFER_TYPE_CONSTANT, D3D11_BIND_CONSTANT_BUFFER }
};

//===============================================================================================
RenderBuffer::RenderBuffer(eBufferType type, uint count, uint stride, void* bufferData /*= nullptr*/)
{
	m_type = type;
	m_count = count;
	m_stride = stride;
	
	CreateBuffer(m_type, m_count, m_stride, bufferData);
}

//-----------------------------------------------------------------------------------------------
RenderBuffer::~RenderBuffer()
{
	if (m_buffer) { m_buffer->Release(); }
}

//-----------------------------------------------------------------------------------------------
void RenderBuffer::CreateBuffer(eBufferType type, uint count, uint stride, void* bufferData)
{
	HRESULT hr = S_OK;
	Renderer* r = Renderer::GetInstance();
	
	if(bufferData == nullptr)
	{
		TODO("Error logging pls")
		return;
	}

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.ByteWidth = stride * count;
	bufferDescription.BindFlags = bindFlagConverter[type];
	bufferDescription.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = bufferData;

	hr = r->m_deviceInterface->CreateBuffer(&bufferDescription, &InitData, &m_buffer);

	if(hr != S_OK)
	{
		TODO("Error logging pls")
		return;
	}
}

