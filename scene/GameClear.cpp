#include "GameClear.h"
#include "../Manager/GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameClear::Initialize() {
	input_ = Input::GetInstance();
	sceneNum = GAMECLEAR_SCENE;
}

void GameClear::Update() {
	// シーンチェンジ
	if (input_->TriggerKey(DIK_SPACE)) {
		SceneTransition::sceneChangeType_ = FADE_IN;
	}
	if (SceneTransition::GetInstance()->GetSceneChangeSignal()) {
		sceneNum = TITLE_SCENE;
	}
}

void GameClear::Draw() {

}

void GameClear::Finalize() {

}