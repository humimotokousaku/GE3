#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;

	input_ = Input::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	viewProjection_.translation_.y = 2.0f;

	// 3Dモデルの生成
	playerModel_.reset(Model::CreateModelFromObj("resources/Player", "player.obj"));
	groundModel_.reset(Model::CreateModelFromObj("resources/Ground", "ground.obj"));
	skydomeModel_.reset(Model::CreateModelFromObj("resources/Skydome", "skydome.obj"));

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get());

	// 地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get(), Vector3{ 0,0,0 });
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), Vector3{ 0,0,0 });
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {
	player_->Update();
	ground_->Update();
	skydome_->Update();

	// カメラ
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
}

void GameScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}