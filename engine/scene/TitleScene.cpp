#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize() {
	input_ = Input::GetInstance();
	viewProjection_.Initialize();

	//// objモデル
	//plane_ = Model::CreateModelFromObj("engine/resources", "plane.obj");
	//axis_ = Model::CreateModelFromObj("engine/resources", "teapot.obj");

	//// Sprite
	//sprite_ = Sprite::Create(UVCHEKER);

	// 線
	line_ = new Line();
	line_->Initialize();

	//// particle
	//particles_ = new Particles();
	//particles_->Initialize();

	t = 0;

	// スプライン曲線制御点（通過点）の初期化
	controlPoints_ = {
		{0,  0,  0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0,  0},
		{30, 0,  0}
	};
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	//plane_->worldTransform.translation_ = Lerp(Vector3{ 0,0,0 }, Vector3{ 5,5,5 }, t);

	// パーティクルの発生位置を指定
	//particles_->SetEmitterPos(Vector3{ 5,5,0 });
	// パーティクルの更新処理
	//particles_->Update();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.1f, -100, 100);
	ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::End();
	ImGui::Begin("Lerp");
	ImGui::DragFloat("t", &t, 0.001f, 0, 1);
	ImGui::End();

#endif
	//sprite_->ImGuiAdjustParameter();
	line_->ImGuiAdjustParameter();
}

void TitleScene::Draw() {
	//axis_->Draw(viewProjection_, UVCHEKER);
	//plane_->Draw(viewProjection_, UVCHEKER);
	//particles_->Draw(viewProjection_, PARTICLE);
	//sprite_->Draw();
	line_->Draw(viewProjection_);
}

void TitleScene::Finalize() {
	delete line_;
	//delete axis_;
	//delete plane_;
	//delete sprite_;
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	//delete particles_;
}