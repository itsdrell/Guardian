#include "Image.hpp"

//===============================================================================================
Image::Image()
{
	m_path = "Unknown";
	m_dimensions = IntVector2(0, 0);
}

//-----------------------------------------------------------------------------------------------
Image::Image(const String& name, const IntVector2& dimension, const Rgba& color)
{
	m_dimensions = dimension;
	m_path = name;

	int amount = dimension.x * dimension.y;

	for (int i = 0; i < amount; i++)
	{
		m_colors.push_back(color);
	}
}

//-----------------------------------------------------------------------------------------------
unsigned char* Image::GetColorCharPointer() const
{
	return (unsigned char*)m_colors.data();
}

//-----------------------------------------------------------------------------------------------
Image::~Image()
{

}
