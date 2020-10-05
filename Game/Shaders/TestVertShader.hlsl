//--------------------------------------------------------------------------------------
// Vertex Shader
// had to do this to get them to compile in VS
// https://developercommunity.visualstudio.com/content/problem/356876/cant-create-new-hlsl-pixelvertexhull-shader-file.html
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color: COLOR;
	float2 Tex : TEXTURE_COORDS;
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color: COLOR;
	float2 Tex : TEXTURE_COORDS;
};

//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.Pos = mul(World, input.Pos);
	output.Pos = mul(View, output.Pos);
	output.Pos = mul(Projection, output.Pos);

	output.Tex = input.Tex;
	output.Color = input.Color;

	return output;
}