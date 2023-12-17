#pragma once
#include "MathStructs.h"
#include "ModelStructs.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <d3d12.h>

class Sphere
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void CreateMaterialResource();

	void CreateWvpResource();

	~Sphere();

	void Initialize();

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	void Release();


	void ImGuiAdjustParameter();

public:
	// Material
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
	// Vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	// カメラ
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraPosResource_;
	Vector3 cameraPosData_;

	//分割数
	const uint32_t kSubdivision = 16;
	// 頂点数
	uint32_t vertexIndex = kSubdivision * kSubdivision * 6;

	// Sphereの画像切り替え
	bool useMonsterBall_ = true;
};