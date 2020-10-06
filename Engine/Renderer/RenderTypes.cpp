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

//-----------------------------------------------------------------------------------------------
enum D3D11_BLEND_OP ToDx11BlendOperation(eBlendOperation operation)
{
	switch (operation)
	{
	case COMPARE_ADD:
		return D3D11_BLEND_OP_ADD;
	case COMPARE_SUBTRACT:
		return D3D11_BLEND_OP_SUBTRACT;
	case COMPARE_REVERSE_SUBTRACT:
		return D3D11_BLEND_OP_REV_SUBTRACT;
	case COMPARE_MIN:
		return D3D11_BLEND_OP_MIN;
	case COMPARE_MAX:
		return D3D11_BLEND_OP_MAX;
	default:
		return D3D11_BLEND_OP_ADD;
	}
}

//-----------------------------------------------------------------------------------------------
D3D11_BLEND ToDx11BlendFactor(eBlendFactor factor)
{
	switch (factor)
	{
	case BLEND_ZERO:
		return D3D11_BLEND_ZERO;
	case BLEND_ONE:
		return D3D11_BLEND_ONE;
	case BLEND_SRC_COLOR:
		return D3D11_BLEND_SRC_COLOR;
	case BLEND_ONE_MINUS_SRC_COLOR:
		return D3D11_BLEND_INV_SRC_COLOR;
	case BLEND_DST_COLOR:
		return D3D11_BLEND_DEST_COLOR;
	case BLEND_ONE_MINUS_DST_COLOR:
		return D3D11_BLEND_INV_DEST_COLOR;
	case BLEND_SRC_ALPHA:
		return D3D11_BLEND_SRC_ALPHA;
	case BLEND_ONE_MINUS_SRC_ALPHA:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case BLEND_DST_ALPHA:
		return D3D11_BLEND_DEST_ALPHA;
	case BLEND_ONE_MINUS_DST_ALPHA:
		return D3D11_BLEND_INV_DEST_ALPHA;
	case BLEND_SRC_ALPHA_SATURATE:
		return D3D11_BLEND_SRC_ALPHA_SAT;
	default:
		return D3D11_BLEND_ZERO;
	}
}