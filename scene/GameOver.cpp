#include "GameOver.h"
#include "../Manager/GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameOver::Initialize() {
	input_ = Input::GetInstance();
	sceneNum = GAMEOVER_SCENE;
}

void GameOver::Update() {
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

void GameOver::Draw() {

}

void GameOver::Finalize() {

}