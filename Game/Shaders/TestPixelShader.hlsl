//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

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
float4 main(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.Tex) * input.Color;
}
