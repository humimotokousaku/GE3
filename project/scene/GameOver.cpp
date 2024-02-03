#include "GameOver.h"
#include "GameManager.h"

void GameOver::Initialize() {
	sceneNum = GAMEOVER_SCENE;
}

void GameOver::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = TITLE_SCENE;
	}

	ImGui::Begin("Current Scene");
	ImGui::Text("GAMEOVER");
	ImGui::Text("SPACE:scene change");
	ImGui::End();
}

void GameOver::Draw() {

}

void GameOver::Finalize() {

}