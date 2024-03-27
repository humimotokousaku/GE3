#include "PostEffectTest.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

// ガウスブラー
float4 GaussianBlur(float2 uv)
{
	float2 texelSize = 1.0f / float2(1280.0f, 720.0f);

	float blurAmount = sin(50 * 0.02f) * 2;

	float4 blurredColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = -4; i <= 4; ++i)
	{
		for (int j = -4; j <= 4; ++j)
		{
			float2 offset = texelSize * float2(i, j) * blurAmount;
			blurredColor += gTexture.Sample(gSampler, uv + offset);
		}
	}

	// カーネルサイズ 9x9なので81で割る
	return blurredColor / 81.0f;
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	// RGBカラー値をグレースケールに変換
	float brightness = dot(textureColor.rgb, float3(0.299, 0.587, 0.114));

	//if (brightness >= 0.1f) 
	//{	// ガウスブラーとグレースケールを足す
	//	output.color = GaussianBlur(input.texcoord) + float4(brightness, brightness, brightness, 1);
	//}
	//else {
	//	output.color = float4(textureColor.rgb, 1);
	//}


	output.color = GaussianBlur(input.texcoord) + float4(brightness, brightness, brightness, 0);
	output.color.a = 1.0;
	//output.color = float4(textureColor.rgb, 1);
	return output;
}