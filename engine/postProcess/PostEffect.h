#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <Windows.h>

class PostEffect : public Sprite {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	//void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
private:
	DirectXCommon* directXCommon_;

	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_;
	// SRV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
};

