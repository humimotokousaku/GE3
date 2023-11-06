#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// 登録したテクスチャの番号
	textureNum_ = GROUND;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = pos;

	// 地面の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)
	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.y = 1.0f;
	worldTransform_.scale_.z = 1.0f;
}

void Ground::Update() {
	// 行列の更新
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureNum_);
}