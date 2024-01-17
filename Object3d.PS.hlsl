#include "Object3d.hlsli"
struct Material
{
	float32_t4 color;
	float4x4 uvTransform;
};
ConstantBuffer<Material>gMaterial :register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct PixelShaderOutput
{
	float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;

	float32_t4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler,transformedUV.xy);
	output.color = gMaterial.color * textureColor;
	return output;
}