#include "GameOver.h"
#include "../Manager/GameManager.h"
#include "SceneTransition/SceneTransition.h"

void GameOver::Initialize() {
	input_ = Input::GetInstance();
	sceneNum = GAMEOVER_SCENE;
}

void GameOver::Update() {
	// シーンチェンジ
	if (input_->TriggerKey(DIK_SPACE)) {
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