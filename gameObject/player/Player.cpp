#include "Player.h"
#include "../../Manager/ImGuiManager.h"
#include "../../math/MyMatrix.h"
#include "../../components/Input.h"
#include <cassert>

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Player::Player() {}
Player::~Player() {}

// Initializeの関数定義
void Player::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// 登録したテクスチャの番号
	playerTexture_ = UVCHEKER;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

// Updateの関数定義
void Player::Update() {
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		// 速さ
		const float speed = 0.3f;
		// しきい値
		const float threshold = 0.7f;
		bool isMoving = false;
		// 移動量
		Vector3 move{
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		if (Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {
			// 移動量の速さを反映
			move = Multiply(speed, Normalize(move));

			// 回転行列
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			// 移動ベクトルをカメラの角度だけ回転
			move = TransformNormal(move, rotateMatrix);

			// 移動量
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);

			// 目標角度の算出
			goalAngle_ = std::atan2(move.x, move.z);
		}
	}
	// 最短角度補間
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, goalAngle_, 0.1f);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, playerTexture_);
}
