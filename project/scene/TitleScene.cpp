#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "Lerp.h"

void TitleScene::Initialize() {
	sceneNum = TITLE_SCENE;
	input_ = Input::GetInstance();
	//viewProjection_.Initialize();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	// objモデル
	ModelManager::GetInstance()->LoadModel("plane.obj");
	plane_ = std::make_unique<Object3D>();
	plane_->Initialize();
	plane_->SetModel("plane.obj");
	plane_->SetCamera(camera_.get());

	ModelManager::GetInstance()->LoadModel("teapot.obj");
	axis_ = std::make_unique<Object3D>();
	axis_->Initialize();
	axis_->SetModel("teapot.obj");
	axis_->SetCamera(camera_.get());

	// Sprite
	sprite_.reset(Sprite::Create(UVCHEKER));

	// particle
	particles_ = std::make_unique<Particles>();
	particles_->Initialize();
	particles_->SetCamera(camera_.get());
	// パーティクルの発生位置を指定
	particles_->SetEmitterPos(Vector3{ 5,5,0 });

	//particles_1 = std::make_unique<Particles>();
	//particles_1->Initialize();
	//particles_1->SetCamera(camera_.get());
	//// パーティクルの発生位置を指定
	//particles_1->SetEmitterPos(Vector3{ 0,0,0 });

	camera_->SetTranslate(Vector3{0,0,-20});

	anim_ = std::make_unique<Animation>();
	anim_->SetAnimData(&plane_->worldTransform.transform.translate, Vector3{ 0,0,0 }, Vector3{10,0,0}, 60, "PlaneAnim0", Easings::EaseOutBack);
	anim_->SetAnimData(&plane_->worldTransform.transform.translate, Vector3{ 10,0,0 }, Vector3{ 0,0,0 }, 60, "PlaneAnim1",Easings::EaseOutBack);
	anim_->SetAnimData(&plane_->worldTransform.transform.translate, Vector3{ 0,0,0 }, Vector3{ 0,5,0 }, 120, "PlaneAnim2",Easings::EaseOutBack);
	anim_->SetAnimData(&plane_->worldTransform.transform.translate, Vector3{ 0,5,0 }, Vector3{ 0,0,0 }, 120, "PlaneAnim3",Easings::EaseOutBack);

	enemy_ = std::make_unique<Enemy>();
	enemy_->SetCamera(camera_.get());
	enemy_->Init();

	ModelManager::GetInstance()->LoadModel("Skydome/skydome.obj");
	player_ = std::make_unique<Player>();
	player_->SetCamera(camera_.get());
	player_->Init();

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetColliderList(enemy_.get());
	collisionManager_->SetColliderList(player_.get());
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//sceneNum = GAME_SCENE;
		anim_->SetIsStart(true);
	}

	anim_->Update();

	//viewProjection_.UpdateViewMatrix();
	//viewProjection_.TransferMatrix();

	camera_->Update();

	player_->Update();
	enemy_->Update();

	// パーティクルの更新処理
	particles_->Update();

	//// パーティクルの更新処理
	//particles_1->Update();

	collisionManager_->CheckAllCollisions();

#ifdef USE_IMGUI
	ImGui::Begin("Animation");
	ImGui::Text("isStart:%d", anim_->GetIsStart());
	ImGui::End();

	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("translation", &camera_->viewProjection_.translation_.x, 0.1f, -100, 100);
	//ImGui::DragFloat3("rotation", &camera_->GetRotate().x, 0.01f, -6.28f, 6.28f);
	//ImGui::End();
	ImGui::Begin("plane");
	ImGui::DragFloat3("translation", &plane_->worldTransform.transform.translate.x, 0.01f, -100, 100);
	ImGui::End();
	ImGui::Begin("axis");
	ImGui::DragFloat3("translation", &axis_->worldTransform.transform.translate.x, 0.01f, -100, 100);
	ImGui::End();

	sprite_->ImGuiAdjustParameter();

	ImGui::Begin("Particles");
	if (ImGui::TreeNode("Particle")) {
		particles_->ImGuiAdjustParameter();
		ImGui::TreePop();
	}
	//if (ImGui::TreeNode("Particle1")) {
	//	particles_1->ImGuiAdjustParameter();
	//	ImGui::TreePop();
	//}


	ImGui::End();

	ImGui::Begin("Current Scene");
	ImGui::Text("TITLE");
	ImGui::Text("SPACE:scene change");
	ImGui::End();
#endif
}

void TitleScene::Draw() {
	axis_->Draw(UVCHEKER);
	plane_->Draw(UVCHEKER);

	enemy_->Draw();
	player_->Draw();

	particles_->Draw(PARTICLE);
	//particles_1->Draw(PARTICLE);
	sprite_->Draw();
}

void TitleScene::Finalize() {
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
}