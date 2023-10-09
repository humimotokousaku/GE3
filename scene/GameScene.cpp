#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;

	input_ = Input::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	viewProjection_.translation_.y = 2.0f;

	// 3Dモデルの生成

	// 自機
	modelFighterBody_.reset(Model::CreateModelFromObj("resources/float_Body", "float_Body.obj"));
	modelFighterHead_.reset(Model::CreateModelFromObj("resources/float_Head", "float_Head.obj"));
	modelFighterL_arm_.reset(Model::CreateModelFromObj("resources/float_L_arm", "float_L_arm.obj"));
	modelFighterR_arm_.reset(Model::CreateModelFromObj("resources/float_R_arm", "float_R_arm.obj"));
	modelFighterHammer_.reset(Model::CreateModelFromObj("resources/Hammer", "Hammer.obj"));
	// 自機モデル
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
		modelFighterR_arm_.get(), modelFighterHammer_.get() };
	// 敵モデル
	modelEnemy_.reset(Model::CreateModelFromObj("resources/Enemy", "Enemy.obj"));
	std::vector<Model*> enemyModels = {
		modelEnemy_.get() };
	// 地面
	groundModel_.reset(Model::CreateModelFromObj("resources/Ground", "ground.obj"));
	// 天球
	skydomeModel_.reset(Model::CreateModelFromObj("resources/Skydome", "skydome.obj"));

	// 自機の生成
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	// 敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get(), Vector3{ 0,0,0 });
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), Vector3{ 0,0,0 });
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransformBase());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();
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
	player_->Draw(viewProjection_, BLACK);
	enemy_->Draw(viewProjection_, BLACK);
}

void GameScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}