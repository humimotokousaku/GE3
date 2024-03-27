#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "Lerp.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	input_ = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Engine/resources/titleName.png");

	// Sprite
	sprite_[0].reset(Sprite::Create("Engine/resources/monsterBall.png"));
	sprite_[0]->SetPos(Vector2{ 200,360 });
	sprite_[1].reset(Sprite::Create("Engine/resources/uvChecker.png"));
	sprite_[1]->SetPos(Vector2{ 1080,360 });
	// 背景
	sprite_[2].reset(Sprite::Create("Engine/resources/uvChecker.png"));
	sprite_[2]->SetIsBackGround();
	sprite_[2]->SetColor(Vector4{ 0,0,0,1 });
	sprite_[2]->SetPos(Vector2{ 640,360 });
	sprite_[2]->SetSize(Vector2{ 1280,720 });
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
	}

	ImGui::Begin("Scene2");
	ImGui::Text("SPACE:Scene Change");
	ImGui::End();
}

void TitleScene::Draw() {
	for (int i = 0; i < 3; i++) {
		sprite_[i]->Draw();
	}
}

void TitleScene::Finalize() {

}