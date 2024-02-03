#include "Object3D.h"
#include "PipelineManager.h"
#include "ModelManager.h"

Object3D::~Object3D() {
	worldTransform.constBuff_.ReleaseAndGetAddressOf();
	delete model_;
}

void Object3D::Initialize(const std::string& filePath) {
	worldTransform.Initialize();
	model_ = new Model();
	model_ = ModelManager::GetInstance()->SetModel(filePath);
}

void Object3D::Draw(ViewProjection viewProjection, uint32_t textureNum) {
	// ワールド座標の更新
	worldTransform.UpdateMatrix();
	/// コマンドを積む
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetRootSignature()[1].Get());
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetGraphicsPipelineState()[1].Get()); // PSOを設定

	// 形状を設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// worldTransform
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuff_->GetGPUVirtualAddress());
	
	// 見た目を描画
	model_->Draw(viewProjection, textureNum);
}