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

	modelSkydome_ = Model::CreateModelFromObj("resources/Skydome", "skydome.obj");

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,0,-40 };

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, playerTexture_);

	// enemyの生成
	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	// enemyの初期化
	enemy_->Initialize(model_, Vector3(0, 3, 30));

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, { 0, 0, 0 });

	// 衝突マネージャーの生成
	collisionManager_ = new CollisionManager();
	collisionManager_->Initialize(player_, enemy_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// enemyの更新
	if (enemy_) {
		enemy_->Update();
	}

	// 天球
	skydome_->Update();

	// 衝突マネージャー(当たり判定)
	collisionManager_->CheckAllCollisions();
}

void GameScene::Draw() {
	player_->Draw(viewProjection_);

	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);
}

void GameScene::Finalize() {
	// 3Dモデル
	delete model_;
	// 自キャラの解放
	delete player_;
	// enemyの解放
	delete enemy_;
	// 衝突マネージャーの解放
	delete collisionManager_;
	// 3Dモデル
	delete modelSkydome_;
	// 天球
	delete skydome_;
	// ビュー(カメラ)の解放
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}