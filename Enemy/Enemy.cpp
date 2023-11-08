#include "Enemy.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

#include "../Player/Player.h"

#include "../Collision/CollisionConfig.h"

#include "../math/Lerp.h"
#include "../math/Matrix4x4.h"

#include "../base/WorldTransform.h"
#include "../base/Model.h"

#include <cassert>
#include <stdio.h>

Enemy::Enemy() { 
	state_ = new EnemyStateApproach();
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	enemyTexture_ = ENEMY;

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = pos;

	// 状態遷移
	state_->Initialize(this);
}

void Enemy::Move(const Vector3 velocity) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
}

void Enemy::Update() { 
	// 状態遷移
	state_->Update(this); 

	// 行列の更新
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Enemy::ChangeState(IEnemyState* pState) {
	delete state_;
	state_ = pState;
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// enemy
	model_->Draw(worldTransform_, viewProjection, enemyTexture_);
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度(正の数だと敵の後ろから弾が飛ぶ)
	const float kBulletSpeed = -0.5f;
	Vector3 velocity{ 0, 0, kBulletSpeed };

	// 自キャラのワールド座標を取得する
	player_->GetWorldPosition();

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, GetWorldPosition(), velocity);
	newBullet->SetPlayer(player_);

	// 弾を登録
	bullets_.push_back(newBullet);
}

void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
