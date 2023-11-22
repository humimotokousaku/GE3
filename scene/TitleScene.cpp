#include "TitleScene.h"
#include "../Manager/ImGuiManager.h"

void TitleScene::Initialize() {
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	sprite_ = Sprite::Create(Vector2(36, 36));
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	sprite_->ImGuiAdjustParameter();
}

void TitleScene::Draw() {
	sprite_->Draw(Vector2(1, 1), textureNum_);
}

void TitleScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	delete sprite_;
}