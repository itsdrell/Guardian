#include "ShaderProgram.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <d3d11_1.h>

//===============================================================================================
ShaderProgram::ShaderProgram(const String& vertexName, const String& pixelName)
{
	String vertexPath = COMPILED_SHADER_LOCATION + vertexName + ".cso";
	String pixelPath = COMPILED_SHADER_LOCATION + pixelName + ".cso";

	CreateVertexAndPixelShader(vertexPath, pixelPath);
	CreateInputLayout();
}

//-----------------------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_vertexShader->Release();

	free(m_vertexBuffer);
	free(m_pixelShader);
}

//-----------------------------------------------------------------------------------------------
void ShaderProgram::CreateVertexAndPixelShader(const String& vertexPath, const String& pixelPath)
{
	HRESULT hr = S_OK;
	Renderer* r = Renderer::GetInstance();

	bool result = LoadCompiledShaderFromFile(pixelPath.c_str(), m_pixelBufferSize, m_pixelBuffer);
	if (result == false)
	{
		return;
	}

	hr = r->m_deviceInterface->CreatePixelShader(m_pixelBuffer, m_pixelBufferSize, nullptr, &m_pixelShader);

	// create vertex shader
	result = LoadCompiledShaderFromFile(vertexPath.c_str(), m_vertexBufferSize, m_vertexBuffer);
	if (result == false)
	{
		return;
	}

	hr = r->m_deviceInterface->CreateVertexShader(m_vertexBuffer, m_vertexBufferSize, nullptr, &m_vertexShader);
}

//-----------------------------------------------------------------------------------------------
// This may not be the best place to do it, also it forces us to only have one type :l 
// but I feel like the shader program should own the ID3D11InputLayout
void ShaderProgram::CreateInputLayout()
{
	HRESULT hr = S_OK;
	Renderer* r = Renderer::GetInstance();
	
	// input layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE_COORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = r->m_deviceInterface->CreateInputLayout(layout, ARRAYSIZE(layout), m_vertexBuffer, m_vertexBufferSize, &m_vertexLayout);
}

//-----------------------------------------------------------------------------------------------
bool ShaderProgram::LoadCompiledShaderFromFile(const String& path, int& sizeOut, char*& out)
{
	TODO("Make a LoadFileIntoBuffer function")
	// I would make this into a function thats like "LoadFileIntoBuffer"
	// and add some error checking

	FILE *fl;
	fopen_s(&fl, path.c_str(), "rb");

	fseek(fl, 0, SEEK_END);
	long len = ftell(fl);

	out = (char*)malloc(len);

	fseek(fl, 0, SEEK_SET);
	fread(out, 1, len, fl);
	fclose(fl);

	sizeOut = len;

	return true;
}