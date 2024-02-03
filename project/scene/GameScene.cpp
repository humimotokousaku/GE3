#include "GameScene.h"
#include "GameManager.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
}

void GameScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = GAMEOVER_SCENE;
	}

	ImGui::Begin("Current Scene");
	ImGui::Text("GAME");
	ImGui::Text("SPACE:scene change");
	ImGui::End();
}

void GameScene::Draw() {

}

void GameScene::Finalize() {

}