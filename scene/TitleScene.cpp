#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"
#include "../math/Lerp.h"
#include "SceneTransition/SceneTransition.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	// カメラの初期化
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,2,-10 };
	viewProjection_.UpdateMatrix();

	// アニメーションするモデルの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.UpdateMatrix();

	// タイトルロゴ
	titleName_ = new Sprite();
	titleName_->Initialize(Vector2{ 0,0 }, Vector2{ 640, 180 }, false);
	titleName_->SetPosition(Vector2{ 320, 200 });

	// ゲームパッド
	gamePad_A_ = new Sprite();
	gamePad_A_->Initialize(Vector2{ 0,0 }, Vector2{ 64, 64 }, false);
	gamePad_A_->SetPosition(Vector2{ 576, 500 });
	alpha_ = 1.0f;

	// 背景
	backGround_ = new Sprite();
	backGround_->Initialize(Vector2{ 0,0 }, Vector2{ 1280, 720 }, true);
	backGround_->SetPosition(Vector2{ 0, 0 });
	backGround_->SetColor(Vector4{ 0.1f, 0.1f, 0.1f, 1 });

	// モデル読み込み
	block_.reset(Model::CreateModelFromObj("resources", "block.obj"));

	isIncrement_ = true;
}

void TitleScene::Update() {
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	// Aボタンを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}
	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = GAME_SCENE;
	}

#pragma region カメラ操作

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//// デッドゾーンの設定
		SHORT rightThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
		const float kRadian = 0.02f;
		viewProjection_.rotation_.y += (float)rightThumbX / SHRT_MAX * kRadian;
	}
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
		SHORT leftThumbZ = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);
		//const float kRadian = 0.02f;
		viewProjection_.translation_.x += (float)leftThumbX / SHRT_MAX * 0.5f;
		viewProjection_.translation_.z += (float)leftThumbZ / SHRT_MAX * 0.5f;
	}

	// Keyboard
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		const float speed = -0.1f;

		Vector3 move = { speed,0,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, viewProjection_.matView);

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		const float speed = 0.1f;

		Vector3 move = { speed,0,0 };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, viewProjection_.matView);

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		const float speed = 0.1f;

		Vector3 move = { 0,0, speed };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, viewProjection_.matView);

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		const float speed = -0.1f;

		Vector3 move = { 0,0, speed };

		// 移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, viewProjection_.matView);

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}

	// keyboard
	if (Input::GetInstance()->PressKey(DIK_W)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { -0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_A)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,-0.01f,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_S)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_D)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,0.01f,0 });
	}

#pragma endregion

	// アニメーション
	AnimUpdate();

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void TitleScene::Draw() {
	backGround_->Draw(PLAYER);
	block_->Draw(worldTransform_, viewProjection_, ENEMY);
	titleName_->Draw(TITLENAME);
	gamePad_A_->Draw(GAMEPAD_A);
}

void TitleScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	block_.reset();
	delete backGround_;
	delete gamePad_A_;
	delete titleName_;
}

void TitleScene::AnimUpdate() {
	// モデルを上下させる
	const float kSpeed = 1.0f / 600.0f;
	theta_ += kSpeed;
	worldTransform_.translation_.y = sinf(theta_ * 30.0f);

	// モデルを回転させる
	worldTransform_.rotation_.y = 6.28f * theta_;

	worldTransform_.UpdateMatrix();

#pragma region タイトル名のアニメーション

	if (isIncrement_) {
		animCount_++;
	}
	else {
		animCount_--;
	}
	// アニメーション
	t_ = EaseOutBack((float)animCount_ / 60);
	Vector2 ease = {
		Lerp(Vector3{ 320,200,0 }, Vector3{ 320,220,0 }, t_).x,
		Lerp(Vector3{ 320,200,0 }, Vector3{ 320,220,0 }, t_).y
	};
	titleName_->SetPosition(ease);

	if (t_ > 1.0f) {
		isIncrement_ = false;
	}
	else if (t_ < 0.0f) {
		isIncrement_ = true;
	}

#pragma endregion

#pragma region Aボタンの点滅

	alpha_ = sinf(theta_ * 30);
	gamePad_A_->SetColor(Vector4{ 1,1,1,alpha_ });

#pragma endregion
}

float TitleScene::EaseOutBack(float x) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * (float)pow(x - 1, 3) + c1 * (float)pow(x - 1, 2);
}