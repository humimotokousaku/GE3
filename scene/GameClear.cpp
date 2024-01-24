#include "GameClear.h"
#include "../Manager/GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameClear::Initialize() {
	input_ = Input::GetInstance();
	sceneNum = GAMECLEAR_SCENE;

	// クリア
	clear_ = new Sprite();
	clear_->Initialize(Vector2{ 0,0 }, Vector2{ 580, 164 }, false);
	clear_->SetPosition(Vector2{ 320, 200 });

	// ゲームパッド
	gamePad_A_ = new Sprite();
	gamePad_A_->Initialize(Vector2{ 0,0 }, Vector2{ 64, 64 }, false);
	gamePad_A_->SetPosition(Vector2{ 576, 500 });

	// 背景
	backGround_ = new Sprite();
	backGround_->Initialize(Vector2{ 0,0 }, Vector2{ 1280, 720 }, true);
	backGround_->SetPosition(Vector2{ 0, 0 });
	backGround_->SetColor(Vector4{ 0.1f, 0.1f, 0.1f, 1 });
}

void GameClear::Update() {
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	// シーンチェンジ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}
	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = TITLE_SCENE;
	}
}

void GameClear::Draw() {
	backGround_->Draw(PLAYER);
	clear_->Draw(CLEAR);
	gamePad_A_->Draw(GAMEPAD_A);
}

void GameClear::Finalize() {
	delete backGround_;
	delete gamePad_A_;
	delete clear_;
}