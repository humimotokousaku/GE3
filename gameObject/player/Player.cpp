#include "Player.h"
#include "../../Manager/ImGuiManager.h"
#include "../../math/MyMatrix.h"
#include "../../components/Input.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "math.h"

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
	worldTransformHammer_.parent_ = parent;
}

void Player::SetCameraParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformBody_.parent_ = parent;
}

void Player::InitializeFloatingGimmick() {
	for (int i = 0; i < kMaxMoveModelParts; i++) {
		floatingParameter_[i] = 0.0f;
	}
}

void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	uint16_t floatingCycle[2]{};
	floatingCycle[0] = 30;
	floatingCycle[1] = 60;
	// 1フレームでのパラメータ加算値
	float step[2]{};
	for (int i = 0; i < kMaxMoveModelParts; i++) {
		step[i] = 2.0f * (float)M_PI / floatingCycle[i];
		// パラメータを1ステップ分加算
		floatingParameter_[i] += step[i];
		// 2πを超えたら0に戻す
		floatingParameter_[i] = (float)std::fmod(floatingParameter_[i], 2.0f * M_PI);
	}
	// 浮遊の振幅<m>
	const float floatingAmplitude = 0.5f;
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_[0]) * floatingAmplitude;

	// 腕の動き
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_[1]) * 0.75f;
	worldTransformR_arm_.rotation_.x = -std::sin(floatingParameter_[1]) * 0.75f;
}

Player::Player() {}
Player::~Player() {}

// Initializeの関数定義
void Player::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	ICharacter::Initialize(models);

	// 引数として受け取ったデータをメンバ変数に記録する
	//models_[kModelIndexBody] = models[kModelIndexBody];
	//models_[kModelIndexHead] = models[kModelIndexHead];
	//models_[kModelIndexL_arm] = models[kModelIndexL_arm];
	//models_[kModelIndexR_arm] = models[kModelIndexR_arm];

	// 登録したテクスチャの番号
	playerTexture_ = BLACK;

	// 腕の座標指定
	worldTransformL_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.x = -1.5f;
	worldTransformL_arm_.translation_.y = 5.0f;
	worldTransformR_arm_.translation_.y = 5.0f;

	// 身体のパーツの親子関係を結ぶ
	SetCameraParent(&worldTransform_);
	//world
	SetParent(&GetWorldTransformBody());

	// 浮遊ギミックの初期化
	InitializeFloatingGimmick();

	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();
}

// Updateの関数定義
void Player::Update() {
	// 初期化
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
			// 攻撃行動
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
			// ダッシュ
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 更新処理
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		// 攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
		// ダッシュ
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	worldTransformHammer_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	// 基底クラスの更新処理
	ICharacter::Update();

	ImGui::Begin("Player");
	ImGui::Text("R_TRIGGER:Attack");
	ImGui::Text("L_TRIGGER:Dash");
	ImGui::End();
}

// Drawの関数定義
void Player::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection,textureHandle);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection,textureHandle);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection, textureHandle);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection, textureHandle);
	if (behavior_ == Behavior::kAttack) {
		models_[kModelIndexWeapon]->Draw(worldTransformHammer_, viewProjection, textureHandle);
	}
}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformHammer_.rotation_.x = 0.0f;
	// 浮遊ギミックの初期化
	InitializeFloatingGimmick();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();
}

void Player::BehaviorRootUpdate() {
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	// Lトリガーを押したらダッシュ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		behaviorRequest_ = Behavior::kDash;
	}
	// Rトリガーを押したら攻撃
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kAttack;
	}

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
			//// 追従カメラはtranslationの数値を見て追ってきているのでBodyの計算結果を代入
			//worldTransform_.translation_ = worldTransformBody_.translation_;
			//// Bodyは上下に揺れているのでyを0にする
			//worldTransform_.translation_.y = 0;

			// 目標角度の算出
			goalAngle_ = std::atan2(move.x, move.z);
		}
	}
	// 最短角度補間
	worldTransformBody_.rotation_.y = LerpShortAngle(worldTransformBody_.rotation_.y, goalAngle_, 0.1f);

	// 浮遊ギミックの更新処理
	UpdateFloatingGimmick();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::BehaviorAttackInitialize() {
	worldTransformL_arm_.rotation_.x = (float)M_PI;
	worldTransformR_arm_.rotation_.x = (float)M_PI;
	worldTransformHammer_.rotation_.x = 0.0f;
	attackAnimationFrame = 0;
}

void Player::BehaviorAttackUpdate() {
	if (attackAnimationFrame < 10) {
		// 腕の挙動
		worldTransformL_arm_.rotation_.x -= 0.05f;
		worldTransformR_arm_.rotation_.x -= 0.05f;

		// 武器の挙動
		worldTransformHammer_.rotation_.x -= 0.05f;
	}
	else if (worldTransformHammer_.rotation_.x <= 2.0f * (float)M_PI / 4) {
		// 腕の挙動
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;
		// 武器の挙動
		worldTransformHammer_.rotation_.x += 0.1f;
	}
	else {
		// 振るまいリクエストをリセット
		behaviorRequest_ = Behavior::kRoot;
	}
	attackAnimationFrame++;
}

void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	// 移動量
	dashMove_ = { 0,0,5 };
	// 移動ベクトルをplayerの身体のの角度だけ回転
	dashMove_ = TransformNormal(dashMove_, worldTransformBody_.matWorld_);
}

void Player::BehaviorDashUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, dashMove_);

	// ダッシュ時間
	const uint32_t behaviorDashTime = 30;

	// 既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}