#include "Player.h"
#include "../Collision/CollisionConfig.h"
#include "../base/WorldTransform.h"
#include "../Manager/ImGuiManager.h"
#include "../math/Matrix4x4.h"
#include "../scene/GameScene.h"
#include <cassert>

Player::Player() {}
Player::~Player() {
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	worldTransform3DReticle_.constBuff_.ReleaseAndGetAddressOf();
	delete sprite2DReticle_;
}

// Initializeの関数定義
void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	playerTexture_ = textureHandle;
	// レティクル
	sprite2DReticle_ = new Sprite();
	sprite2DReticle_->Initialize(Vector2{0,0}, Vector2{32,32}, false);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	worldTransform_.translation_ = pos;

	// スプライトの現在座標を取得
	spritePosition_ = sprite2DReticle_->GetPosition();
}

// Updateの関数定義
void Player::Update(const ViewProjection& viewProjection) {
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	/// 移動処理↓

	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		move.x += (float)leftThumbX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)leftThumbY / SHRT_MAX * kCharacterSpeed;
	}
	// 移動限界座標
	const Vector2 kMoveLimit = { 40 - 10, 30 - 15 };
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimit.x);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimit.x);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimit.y);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimit.y);
	// 座標移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	/// 移動処理↑

	// 旋回処理
	Rotate();

	worldTransform_.UpdateMatrix();

	// 弾の処理
	Attack();

	// 3Dレティクルの配置
	Deploy3DReticle();
	// 2Dレティクルを配置
	Deploy2DReticle(viewProjection);

//	// 2Dレティクルの移動
//	Vector2 joyRange{};
//	// ジョイスティック状態取得
//	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
//		// デッドゾーンの設定
//		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
//		SHORT rightThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRY);
//		joyRange.x += (float)rightThumbX / SHRT_MAX * 10.0f;
//		joyRange.y += (float)rightThumbY / SHRT_MAX * 10.0f;
//		spritePosition_.x += joyRange.x;
//		spritePosition_.y -= joyRange.y;
//		// スプライトへの座標変更を反映
//		sprite2DReticle_->SetPosition(spritePosition_);
//	}
//#pragma region 2Dレティクルから3Dレティクルに変換
//
//	// ビューポート行列
//	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, (float)WinApp::kClientWidth_, (float)WinApp::kClientHeight_, 0, 1);
//	// ビュープロジェクションビューポート合成行列
//	Matrix4x4 matVPV = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
//	// 合成行列の逆行列を計算する
//	Matrix4x4 matInverseVPV = Inverse(matVPV);
//
//	// スクリーン座標
//	Vector3 posNear = Vector3((float)sprite2DReticle_->GetPosition().x + 16, (float)sprite2DReticle_->GetPosition().y + 16, 0);
//	Vector3 posFar = Vector3((float)sprite2DReticle_->GetPosition().x + 16, (float)sprite2DReticle_->GetPosition().y + 16, 1);
//
//	// スクリーン座標系からワールド座標系へ
//	posNear = Transforms(posNear, matInverseVPV);
//	posFar = Transforms(posFar, matInverseVPV);
//	// マウスレイの方向
//	Vector3 mouseDirection = Subtract(posFar, posNear);
//	mouseDirection = Normalize(mouseDirection);
//	// カメラから照準オブジェクトの距離
//	const float kDistanceTestObject = 100.0f;
//	// 3Dレティクルを2Dカーソルに配置
//	worldTransform3DReticle_.translation_.x = posNear.x - mouseDirection.x * kDistanceTestObject;
//	worldTransform3DReticle_.translation_.y = posNear.y - mouseDirection.y * kDistanceTestObject;
//	worldTransform3DReticle_.translation_.z = posNear.z - mouseDirection.z * kDistanceTestObject;
//
//	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルの移動
	Vector2 joyRange{};
	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
		SHORT rightThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRY);
		joyRange.y += (float)rightThumbY / SHRT_MAX / 62.8f;
		joyRange.x += (float)rightThumbX / SHRT_MAX / 62.8f;	
		worldTransform_.rotation_.x -= joyRange.y;
		worldTransform_.rotation_.y += joyRange.x;
		worldTransform_.UpdateMatrix();
	}

#pragma endregion

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

//ImGui::Begin("GamePad_Info");
//	ImGui::Text("L_Stick:MovePlayer\nR_Stick:MoveReticle\nR_Trigger:Shot");
//	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.001f, -100, 100);
//	ImGui::DragFloat3("Player.rotation", &worldTransform_.rotation_.x, 0.001f, -100, 100);
//	ImGui::DragFloat3("Reticle.translation", &worldTransform3DReticle_.translation_.x, 0.001f, -100, 100);
//	ImGui::DragFloat2("2Drethicle.translation", &spritePosition_.x, 1, -1000, 1000);
//	ImGui::End();

#endif // _DEBUG

	
}

// Drawの関数定義
void Player::Draw(ViewProjection& viewProjection) {
	// player
	model_->Draw(worldTransform_, viewProjection, playerTexture_);
	// 3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection, UVCHEKER);
}

void Player::DrawUI() {
	sprite2DReticle_->Draw(RETICLE);
}

// playerの回転
void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PressKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PressKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

// 攻撃
void Player::Attack() {
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 速度ベクトルを自機の向きに合わせて回転させる
		Vector3 worldPos = GetWorldPosition();
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		// 自機から照準オブジェクトへのベクトル
		Vector3 worldReticlePos = {
			worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
			worldTransform3DReticle_.matWorld_.m[3][2] };
		velocity = Subtract(worldReticlePos, worldPos);
		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldPos, velocity);

		// 弾を登録
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::Deploy3DReticle() {
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset{ 0, 0, 1.0f };
	// 自機のワールド行列の回転を反映する
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset);
	// ベクトルの長さを整える
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;

	worldTransform3DReticle_.UpdateMatrix();
}

void Player::Deploy2DReticle(const ViewProjection& viewProjection) {
	// 3Dレティクルのワールド座標を取得
	Vector3 positionReticle = GetWorld3DReticlePosition();

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, (float)WinApp::kClientWidth_, (float)WinApp::kClientHeight_, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport{};
	matViewProjectionViewport =
		Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド→スクリーン座標変換
	positionReticle = Transforms(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x - 16, positionReticle.y - 16));
}

void Player::OnCollision() {}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorld3DReticlePosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
