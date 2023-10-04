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
	player_->Initialize(playerModel_.get(), UVCHEKER);
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get(), Vector3{ 0,0,0 });
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), Vector3{ 0,0,0 });
}

void GameScene::Update() {
	// Keyboard
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		const float speed = -0.2f;

		Vector3 move = { speed,0,0 };

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		const float speed = 0.1f;

		Vector3 move = { speed,0,0 };

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		const float speed = 0.1f;

		Vector3 move = { 0,0, speed };

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		const float speed = -0.1f;

		Vector3 move = { 0,0, speed };

		viewProjection_.translation_ = Add(viewProjection_.translation_, move);
	}

	// keyboard
	if (Input::GetInstance()->PressKey(DIK_W)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { -0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_A)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,-0.01f,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_S)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0.01f,0,0 });
	}
	if (Input::GetInstance()->PressKey(DIK_D)) {
		viewProjection_.rotation_ = Add(viewProjection_.rotation_, { 0,0.01f,0 });
	}

	player_->Update();
	ground_->Update();
	skydome_->Update();

	viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
}

void GameScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}