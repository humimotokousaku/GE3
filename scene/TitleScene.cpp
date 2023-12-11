#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	sprite_ = Sprite::Create(MONSTERBALL);
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	sprite_->ImGuiAdjustParameter();
}

void TitleScene::Draw() {
	sprite_->Draw();
}

void TitleScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	delete sprite_;
}