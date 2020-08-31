#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr const char* COMPILED_SHADER_LOCATION = "Data\\Shaders\\Compiled\\";

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class ShaderProgram
{
public:
	ShaderProgram(const String& vertexName, const String& pixelName);
	~ShaderProgram();

private:
	void CreateVertexAndPixelShader(const String& vertexPath, const String& pixelPath);
	void CreateInputLayout();
	bool LoadCompiledShaderFromFile(const String& path, int& sizeOut, char*& out);

public:
	// Its annoying we have to keep buffer and size but CreateInputLayout requires us to hold onto them??? :l 
	ID3D11VertexShader*		m_vertexShader = nullptr;
	char*					m_vertexBuffer = nullptr;
	int						m_vertexBufferSize = 0;

	ID3D11PixelShader*		m_pixelShader = nullptr;
	char*					m_pixelBuffer = nullptr;
	int						m_pixelBufferSize = 0;

	// this could be its own class???
	ID3D11InputLayout*		m_vertexLayout = nullptr;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [8/22/2020]
//====================================================================================