#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class Image;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Texture
{
	// maybe change this to an asset manager later?
	friend class Renderer;

public:
	Texture();
	Texture(const String& path);
	~Texture();

private:
	void PopulateFromData(unsigned char* imageData, int numberOfComponents);

private:
	Texture* CreateFromImage(const Image& theImage);

public:
	IntVector2	m_dimensions;

// should be private once we add methods to create and get in renderer
public:
	ID3D11Texture2D*			m_texture = nullptr;
	ID3D11ShaderResourceView*	m_textureView = nullptr;

	// Make this a Sampler class?
	ID3D11SamplerState*			m_textureSampler = nullptr;
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