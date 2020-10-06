#include "RenderTypes.hpp"
#include <d3d11_1.h>

//===============================================================================================
D3D11_CULL_MODE ToDx11CullMode(eCullMode mode)
{
	switch (mode)
	{
	case CULLMODE_BACK:
		return D3D11_CULL_BACK;
	case CULLMODE_FRONT:
		return D3D11_CULL_FRONT;
	case CULLMODE_NONE:
		return D3D11_CULL_NONE;
	default:
		return D3D11_CULL_NONE;
	}
}

//-----------------------------------------------------------------------------------------------
D3D11_FILL_MODE ToDx11FillMode(eFillMode mode)
{
	switch (mode)
	{
	case FILLMODE_SOLID:
		return D3D11_FILL_SOLID;
	case FILLMODE_WIRE:
		return D3D11_FILL_WIREFRAME;
	default:
		return D3D11_FILL_SOLID;
	}
}