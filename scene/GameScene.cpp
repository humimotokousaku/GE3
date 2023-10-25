#include "GameScene.h"
#include "../Manager/GameManager.h"
#include "../Manager/ImGuiManager.h"
#include "../Manager/TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	playerTexture_ = PLAYER;
	// 3Dモデルの生成
	model_ = Model::CreateModelFromObj("resources","block.obj");

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	viewProjection_.translation_ = { 0,0,-40 };

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, playerTexture_);

	// enemyの生成
	enemy_ = new Enemy();
	// enemyの初期化
	enemy_->Initialize(model_, Vector3(0, 3, 30));
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// enemyの更新
	if (enemy_) {
		enemy_->Update();
	}
}

void GameScene::Draw() {
	player_->Draw(viewProjection_);

	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}
}

void GameScene::Finalize() {
	// 3Dモデル
	delete model_;
	// 自キャラの解放
	delete player_;
	// enemyの解放
	delete enemy_;
	// ビュー(カメラ)の解放
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}