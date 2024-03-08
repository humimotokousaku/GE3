#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "Lerp.h"

void TitleScene::Initialize() {
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Engine/resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Engine/resources/monsterBall.png");
	uvcheckerTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/uvChecker.png");
	monsterBallTexture_ = TextureManager::GetInstance()->GetSrvIndex("Engine/resources/monsterBall.png");

	// objモデル
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("block.obj");

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(Vector3{ 0,5,-20 });

	// 平面
	plane_ = std::make_unique<Object3D>();
	plane_->Initialize();
	plane_->SetModel("plane.obj");
	plane_->SetCamera(camera_.get());
	// 立体的な線
	axis_ = std::make_unique<Object3D>();
	axis_->Initialize();
	axis_->SetModel("axis.obj");
	axis_->SetCamera(camera_.get());

#pragma region その他の処理

	sceneNum = TITLE_SCENE;
	input_ = Input::GetInstance();
	//viewProjection_.Initialize();
	//world_.Initialize();
	//world_.translation_ = { 0,0,0 };

	// Sprite
	//sprite_.reset(Sprite::Create("Engine/resources/uvChecker.png"));

	// particle
	//particles_ = std::make_unique<Particles>();
	//particles_->Initialize();
	//particles_->SetCamera(camera_.get());
	//// パーティクルの発生位置を指定
	//particles_->SetEmitterPos(Vector3{ 5,5,0 });
	//particles_1 = std::make_unique<Particles>();
	//particles_1->Initialize();
	//particles_1->SetCamera(camera_.get());
	//// パーティクルの発生位置を指定
	//particles_1->SetEmitterPos(Vector3{ 0,0,0 });

	// アニメーション
	//anim_ = std::make_unique<Animation>();
	//anim_->SetAnimData(&plane_->worldTransform.translation_, Vector3{ 0,0,0 }, Vector3{ 10,0,0 }, 60, "PlaneAnim0", Easings::EaseOutBack);
	//anim_->SetAnimData(&plane_->worldTransform.translation_, Vector3{ 10,0,0 }, Vector3{ 0,0,0 }, 60, "PlaneAnim1", Easings::EaseOutBack);
	//anim_->SetAnimData(&plane_->worldTransform.translation_, Vector3{ 0,0,0 }, Vector3{ 0,5,0 }, 120, "PlaneAnim2", Easings::EaseOutBack);
	//anim_->SetAnimData(&plane_->worldTransform.translation_, Vector3{ 0,5,0 }, Vector3{ 0,0,0 }, 120, "PlaneAnim3", Easings::EaseOutBack);

	// 自機
	player_ = std::make_unique<Player>();
	player_->Initialize(camera_.get());
	// 敵
	for (int i = 0; i < 2; i++) {
		enemy_[i] = std::make_unique<Enemy>();
		enemy_[i]->Initialize(camera_.get());
	}

	// 当たり判定
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetColliderList(player_.get());
	for (int i = 0; i < 2; i++) {
		collisionManager_->SetColliderList(enemy_[i].get());
	}

#pragma region 各オブジェクトのOBBを設定
	// OBB
	OBB obb;
	obb.m_Pos = { 0,0,0 };
	obb.m_fLength = { 1,1,1 };
	obb.m_NormaDirect[0] = { 1,0,0 };
	obb.m_NormaDirect[1] = { 0,1,0 };
	obb.m_NormaDirect[2] = { 0,0,1 };
	// Enemy用
	OBB obbA;
	obbA.m_Pos = { 10,0,0 };
	obbA.m_fLength = { 1,1,1 };
	obbA.m_NormaDirect[0] = { 1,0,0 };
	obbA.m_NormaDirect[1] = { 0,1,0 };
	obbA.m_NormaDirect[2] = { 0,0,1 };
	OBB obbB;
	obbB.m_Pos = { -10,0,0 };
	obbB.m_fLength = { 1,1,1 };
	obbB.m_NormaDirect[0] = { 1,0,0 };
	obbB.m_NormaDirect[1] = { 0,1,0 };
	obbB.m_NormaDirect[2] = { 0,0,1 };

	player_->SetOBB(obb);
	enemy_[0]->SetOBB(obbA);
	enemy_[0]->SetWorldPosition(obbA.m_Pos);
	enemy_[1]->SetOBB(obbB);
	enemy_[1]->SetWorldPosition(obbB.m_Pos);
#pragma endregion

#pragma endregion
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//sceneNum = GAME_SCENE;
		//anim_->SetIsStart(true);
	}

	//anim_->Update();

	player_->Update();

	for (int i = 0; i < 2; i++) {
		enemy_[i]->Update();
		// 狙う対象に身体を向ける
		//float radian = atan2(player_->model_->worldTransform.translation_.x - enemy_[i]->model_->worldTransform.translation_.x, player_->model_->worldTransform.translation_.z - enemy_[i]->model_->worldTransform.translation_.z);
		//enemy_[i]->model_->worldTransform.rotation_.y = radian;
	}

	//world_.UpdateMatrix();
	//viewProjection_.UpdateViewMatrix();
	//viewProjection_.TransferMatrix();

	// パーティクルの更新処理
	//particles_->Update();

	// パーティクルの更新処理
	//particles_1->Update();

	// 当たり判定
	collisionManager_->CheckAllCollisions();

#ifdef USE_IMGUI
	ImGui::Begin("Animation");
	//ImGui::Text("isStart:%d", anim_->GetIsStart());
	ImGui::End();

	//ImGui::Begin("Camera");
	//ImGui::DragFloat3("translation", &camera_->GetViewProjection().transform.translate.x, 0.1f, -100, 100);
	//ImGui::DragFloat3("rotation", &camera_->viewProjection_.rotation_.x, 0.01f, -6.28f, 6.28f);
	//ImGui::End();
	ImGui::Begin("plane");
	ImGui::DragFloat3("translation", &plane_->worldTransform.transform.translate.x, 0.01f, -100, 100);
	ImGui::End();
	ImGui::Begin("axis");
	ImGui::DragFloat3("translation", &axis_->worldTransform.transform.translate.x, 0.01f, -100, 100);
	ImGui::End();

	//sprite_->ImGuiAdjustParameter();

	ImGui::Begin("Particles");
	//if (ImGui::TreeNode("Particle")) {
	//	particles_->ImGuiAdjustParameter();
	//	ImGui::TreePop();
	//}
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
	axis_->Draw(uvcheckerTexture_);
	plane_->Draw(uvcheckerTexture_);
#pragma region その他の処理
	//testWater_->Draw(world_, viewProjection_);
	player_->Draw(uvcheckerTexture_);
	for (int i = 0; i < 2; i++) {
		enemy_[i]->Draw(monsterBallTexture_);
	}
	//particles_->Draw(PARTICLE);
	//particles_1->Draw(PARTICLE);
	//sprite_->Draw();
#pragma endregion
}

void TitleScene::Finalize() {
	//viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	//world_.constBuff_.ReleaseAndGetAddressOf();
}