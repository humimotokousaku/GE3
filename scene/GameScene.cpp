#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデルの生成
	model_.reset(Model::CreateModelFromObj("resources", "block.obj"));

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(model_.get(),UVCHEKER);
}

void GameScene::Update() {
	player_->Update();
}

void GameScene::Draw() {
	player_->Draw(viewProjection_);
}

void GameScene::Finalize() {
	//model_.reset();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}