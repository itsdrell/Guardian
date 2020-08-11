//--------------------------------------------------------------------------------------
// Vertex Shader
// had to do this to get them to compile in VS
// https://developercommunity.visualstudio.com/content/problem/356876/cant-create-new-hlsl-pixelvertexhull-shader-file.html
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos : POSITION, float4 Color: COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = Pos;
	output.Pos = mul(Pos, World); // object to world
	output.Pos = mul(output.Pos, View); // world to view
	output.Pos = mul(output.Pos, Projection); // view to projection
	output.Color = Color;

	return output;
}