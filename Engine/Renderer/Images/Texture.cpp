#include "Texture.hpp"
#include "Engine/ThirdParty/stbi/stb_image.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/Image.hpp"
#include <d3d11_1.h>

//===============================================================================================
Texture::Texture(const String& path)
{
	int numberOfComponents = 0;
	int numberOfComponentsRequested = 0;

	unsigned char* imageData = stbi_load(path.c_str(), &m_dimensions.x, &m_dimensions.y, &numberOfComponents, numberOfComponentsRequested);

	PopulateFromData(imageData, numberOfComponents);

	stbi_image_free(imageData);
}

//-----------------------------------------------------------------------------------------------
Texture::Texture()
{
	m_dimensions = IntVector2(0, 0);
}

//-----------------------------------------------------------------------------------------------
Texture::~Texture()
{
	if (m_texture) m_texture->Release();
	if (m_textureSampler) m_textureSampler->Release();
	if (m_textureSampler) m_textureSampler->Release();
}

//-----------------------------------------------------------------------------------------------
void Texture::PopulateFromData(unsigned char* imageData, int numberOfComponents)
{
	HRESULT hr = S_OK;
	Renderer* r = Renderer::GetInstance();
	
	// Create the texture resource (https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-create)
	// https://www.rastertek.com/dx11s2tut05.html
	// This is creating a default texture we are going to fill with data
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = m_dimensions.x;
	textureDesc.Height = m_dimensions.y;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = r->m_deviceInterface->CreateTexture2D(&textureDesc, NULL, &m_texture);
	r->m_deviceImmediateContext->UpdateSubresource(m_texture, 0, NULL, imageData, (textureDesc.Width * 4) * sizeof(unsigned char), 0);

	// Create the Shader Resource View for the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = r->m_deviceInterface->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_textureView);
	r->m_deviceImmediateContext->GenerateMips(m_textureView);


	TODO("Please create a sampler class and make one here!")
	// Create the sampler description (linear)
	D3D11_SAMPLER_DESC sampleDescription = {};
	sampleDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDescription.MinLOD = 0;
	sampleDescription.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler
	hr = r->m_deviceInterface->CreateSamplerState(&sampleDescription, &m_textureSampler);
}

//-----------------------------------------------------------------------------------------------
Texture* Texture::CreateFromImage(const Image& theImage)
{
	m_dimensions = theImage.m_dimensions;

	// The image class calls the flip command so we don't have to worry about it!
	unsigned char* imageBuffer = theImage.GetColorCharPointer();
	PopulateFromData((unsigned char*)imageBuffer, 4);

	return this;
}

