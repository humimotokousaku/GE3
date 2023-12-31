#pragma once
//
//#include <string>
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <dxgidebug.h>
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//#include <dxgidebug.h>
//#pragma comment(lib, "dxguid.lib")
//#include <dxcapi.h>
//#pragma comment(lib, "dxcompiler.lib")

#include "../base/DirectXCommon.h"
#include "../math/Vector4.h"
#include "../math/Matrix4x4.h"
#include "../Transform.h"
#include "../VertexData.h"
//#include "components/camera/Camera.h"
#include "../Material.h"
//#include "TransformationMatrix.h"
#include <wrl.h>
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"

class Cube
{
public:
	/// 
	/// Default Method
	/// 
	Cube();

	~Cube();
	// 初期化
	void Initialize();

	// 三角形描画
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	// Getter
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource_.Get(); }

	// Setter
	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU) { textureSrvHandleGPU_ = textureSrvHandleGPU; }

	// Resource生成
	const Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	// VertexResourceの生成
	void CreateVertexResource();

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// MaterialResourceの生成
	void CreateMaterialResource();

	// TransformationMatrix用のResourceを生成
	void CreateWvpResource();

	/// 
	/// User Method
	/// 
	void ApplyGlobalVariables();

	void ImGuiAdjustParameter();

public:
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;
	Transform transform_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
};

