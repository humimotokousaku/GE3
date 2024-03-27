#include "GameScene.h"
#include "GameManager.h"

GameScene::GameScene() {

}

void GameScene::Initialize() {
	sceneNum = GAME_SCENE;

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Engine/resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Engine/resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Engine/resources/circle.png");
	TextureManager::GetInstance()->LoadTexture("Engine/resources/gameplay.png");
	// srvの番号取得
	uvcheckerTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/uvChecker.png");
	monsterBallTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/monsterBall.png");
	particleTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/circle.png");
	gamePlayTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/gameplay.png");

	// objモデル
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(Vector3{ 0,5,-20 });

	/// particle
	particles_ = std::make_unique<Particles>();
	particles_->Initialize();
	particles_->SetCamera(camera_.get());
	// パーティクルの発生位置を指定
	particles_->SetEmitterPos(Vector3{ 4,5,0 });
	particles_1 = std::make_unique<Particles>();
	particles_1->Initialize();
	particles_1->SetCamera(camera_.get());
	// パーティクルの発生位置を指定
	particles_1->SetEmitterPos(Vector3{ -4,5,0 });

	// Sprite
	sprite_[0].reset(Sprite::Create("Engine/resources/uvChecker.png"));
	sprite_[0]->SetIsBackGround();
	sprite_[0]->SetColor(Vector4{ 0,0,0,1 });
	sprite_[0]->SetPos(Vector2{ 640,360 });
	sprite_[0]->SetSize(Vector2{ 1280,720 });

	// 平面
	plane_ = std::make_unique<Object3D>();
	plane_->Initialize();
	plane_->SetModel("plane.obj");
	plane_->SetCamera(camera_.get());
	plane_->SetIsLighting(false);
	plane_->worldTransform.transform.translate = { 8,0,0 };
	// 立体的な線
	axis_ = std::make_unique<Object3D>();
	axis_->Initialize();
	axis_->SetModel("axis.obj");
	axis_->SetCamera(camera_.get());
	axis_->SetIsLighting(false);
	axis_->worldTransform.transform.translate = { -8,0,0 };
}

void GameScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = TITLE_SCENE;
	}
	// パーティクルの更新処理
	particles_->Update();
	// パーティクルの更新処理
	particles_1->Update();

	if (Input::GetInstance()->PressKey(DIK_RIGHT)) {
		camera_->transform_.translate.x += 0.1f;
	}
	if (Input::GetInstance()->PressKey(DIK_LEFT)) {
		camera_->transform_.translate.x -= 0.1f;
	}
	if (Input::GetInstance()->PressKey(DIK_UP)) {
		camera_->transform_.translate.z += 0.1f;
	}
	if (Input::GetInstance()->PressKey(DIK_DOWN)) {
		camera_->transform_.translate.z -= 0.1f;
	}

	if (Input::GetInstance()->PressKey(DIK_S)) {
		camera_->transform_.rotate.x += 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_W)) {
		camera_->transform_.rotate.x -= 0.01f;
	}
	if (Input::GetInstance()->PressKey(DIK_D)) {
		camera_->transform_.rotate.y += 0.02f;
	}
	if (Input::GetInstance()->PressKey(DIK_A)) {
		camera_->transform_.rotate.y -= 0.02f;
	}

#ifdef USE_IMGUI
	ImGui::Begin("Scene1");
	ImGui::Text("SPACE:Scene Change");
	ImGui::Text("Camera Info\nArrow:Move\nWASD:Rotation");
	ImGui::End();
#endif
}

void GameScene::Draw() {
	axis_->Draw(uvcheckerTexture_);
	plane_->Draw(uvcheckerTexture_);

	for (int i = 0; i < 1; i++) {
		sprite_[i]->Draw();
	}	

	particles_->Draw(particleTexture_);
	particles_1->Draw(monsterBallTexture_);
}

void GameScene::Finalize() {

}