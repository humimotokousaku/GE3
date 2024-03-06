#pragma once
#include "d3dx12.h"
#include "DirectXTex.h"
#include "DirectXCommon.h"
#include "MathStructs.h"
#include "ModelStructs.h"
#include <string>
#include <d3d12.h>
#include <wrl.h>

enum TextureName {
	UVCHEKER,
	MONSTERBALL,
	TESTBLOCK,
	PARTICLE,
	COUNTTEXTURE	// テクスチャ数を数える用
};

class TextureManager
{
private:
	// テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metdata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:
	// シングルトン
	static TextureManager* GetInstance();

	// デストラクタ
	~TextureManager() = default;

	// 初期化
	void Initialize();

	// 解放処理
	void Finalize();

	// COMの終了処理
	void ComUninit();
	// COMの初期化
	void ComInit();

	// Textureを読む
	void LoadTexture(const std::string& filePath);

	/// Getter
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	// textureResource
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureResource(uint32_t index) { return textureDatas_[index].resource.Get(); }

private:
	// DirectX12のTextureResourceを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	// 中間リソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	// TextureResourceにデータを転送する
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);

public:// 定数
	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;

private:
	// テクスチャデータ
	std::vector<TextureData> textureDatas_;
};
