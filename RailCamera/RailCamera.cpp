#pragma once
#include "RailCamera.h"
#include "../math/Matrix4x4.h"
#include "../Manager/ImGuiManager.h"

void RailCamera::Initialize(WorldTransform worldTransform, const Vector3& radian) {
	// ワールドトランスフォームの初期設定
	// 引数で受け取った初期座標をセット
	worldTransform_ = worldTransform;
	worldTransform_.translation_.x = 5;
	worldTransform_.translation_.y = 5;
	worldTransform_.translation_.z = -30;

	// 引数で受け取った初期座標をセット
	worldTransform_.rotation_ = radian;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update(Vector3 target) {
	Vector3 velocity = Subtract(target, worldTransform_.translation_);
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	// 横軸方向の長さを求める
	float velocityXZ;
	velocityXZ = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	// X軸周りの角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity.y, velocityXZ);

	// 行列の更新
	worldTransform_.UpdateMatrix();
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();

#endif // _DEBUG
}