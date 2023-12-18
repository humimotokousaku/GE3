#include "PostEffect.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "WinApp.h" 

PostEffect::PostEffect() : Sprite (UVCHEKER)
{

}

void PostEffect::Initialize() {
	directXCommon_ = DirectXCommon::GetInstance();
	HRESULT result;

	// テクスチャリソースの設定
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::kClientWidth_,
		(UINT)WinApp::kClientHeight_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	// テクスチャバッファの生成
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	result = directXCommon_->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff_)
	);
	assert(SUCCEEDED(result));

	// テクスチャの設定
	// 画素数(1280 x 720 = 921600)
	const UINT pixelCount = WinApp::kClientWidth_ * WinApp::kClientHeight_;
	// 画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WinApp::kClientWidth_;
	// 画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WinApp::kClientHeight_;
	// 画像イメージ
	UINT* img = new UINT[pixelCount];
	for (UINT i = 0; i < pixelCount; i++) {
		img[i] = 0xff0000ff;
	}

	// テクスチャバッファにデータ転送
	result = texBuff_->WriteToSubresource(
		0, nullptr,
		img, 
		rowPitch,
		depthPitch
	);
	assert(SUCCEEDED(result));
	delete[] img;

	// SRV用のデスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	// SRV用のデスクリプタヒープを生成
	result = directXCommon_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));
	// SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// デスクリプタヒープにSRV作成
	directXCommon_->GetDevice()->CreateShaderResourceView(texBuff_.Get(),
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::Draw() {
	// ワールド座標の更新
	worldTransform_.UpdateMatrix();

	/// コマンドを積む
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetRootSignature()[0].Get());
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(psoManager_->GetGraphicsPipelineState()[0].Get()); // PSOを設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 描画用のDescriptorHeapの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { descHeapSRV_.Get() };
	directXCommon_->GetCommandList()->SetDescriptorHeaps(1, ppHeaps->GetAddressOf());

	// VBVを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); 
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	// worldTransform
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff_->GetGPUVirtualAddress());
	// viewProjection
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, viewProjection_.constBuff_->GetGPUVirtualAddress());
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraPosResource_.Get()->GetGPUVirtualAddress());

	/// DescriptorTableの設定
	// texture
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());
	// material
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// ライティング
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, DirectionalLight::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, PointLight::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(7, SpotLight::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());


	// 描画(DrawCall/ドローコール)。6頂点で1つのインスタンス
	DirectXCommon::GetInstance()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

D3D12_CPU_DESCRIPTOR_HANDLE PostEffect::GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE PostEffect::GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}