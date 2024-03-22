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

//// ガウシアンぼかしを計算する関数
//float GaussianBlur(float2 texCoord, Texture2D inputTexture, float sigma)
//{
//	float sum = 0.0;
//	float totalWeight = 0.0;
//
//	// ガウス関数のカーネルサイズ
//	int kernelSize = 5;
//	int halfSize = (kernelSize - 1) / 2;
//
//	for (int y = -halfSize; y <= halfSize; ++y)
//	{
//		for (int x = -halfSize; x <= halfSize; ++x)
//		{
//			float2 offset = float2(x, y);
//			float weight = exp(-(dot(offset, offset)) / (2.0 * sigma * sigma)) / (2 * 3.14159 * sigma * sigma);
//			float2 texOffset = texCoord + offset / 1280.0;
//
//			sum += inputTexture.SampleLevel(inputSampler, texOffset, 0).r * weight;
//			totalWeight += weight;
//		}
//	}
//
//	return sum / totalWeight;
//}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	/// グレースケール
	// RGBカラー値をグレースケールに変換
	float brightness = dot(textureColor.rgb, float3(0.299, 0.587, 0.114));
	// 明るさが閾値よりも大きい場合、ピクセルを明るいとマーク
	if (brightness >= 0.5) {
		//output.color = float4(brightness, brightness, brightness, 1.0); // ここでは白色を返すが、実際には必要に応じて適切な処理を行う
		// ぼかし
		float32_t4 color = { 0,0,0,0 };
		float u = 1.0 / 1280.0;
		float v = 1.0 / 720.0;

		color += gTexture.Sample(gSampler, input.texcoord + float2(-u, -v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, -v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, -v));

		color += gTexture.Sample(gSampler, input.texcoord + float2(-u, v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, v));

		color += gTexture.Sample(gSampler, input.texcoord + float2(-u, v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, v));
		color += gTexture.Sample(gSampler, input.texcoord + float2(u, v));

		output.color = color / 9.0;
		output.color.a = 1.0;
	}
	else {
		output.color = float4(textureColor.rgb, 1);
	}








	//output.color = float4(textureColor.rgb, 1);
	return output;
}