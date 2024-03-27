#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "Lerp.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	input_ = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Engine/resources/titleName.png");
	titleNameTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/titleName.png");

	// Sprite
	sprite_.reset(Sprite::Create("Engine/resources/titleName.png"));
	sprite_->SetPos(Vector2{ 640,360 });
	sprite_->SetSize(Vector2{ 1280,720 });
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
	}

	ImGui::Begin("Current Scene");
	ImGui::Text("TITLE");
	ImGui::Text("SPACE:scene change");
	ImGui::End();
}

void TitleScene::Draw() {
	sprite_->Draw();
}

void TitleScene::Finalize() {

}