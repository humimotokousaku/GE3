#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize() {
	input_ = Input::GetInstance();
	viewProjection_.Initialize();

	// objモデル
	plane_ = Model::CreateModelFromObj("engine/resources", "plane.obj");
	axis_ = Model::CreateModelFromObj("engine/resources", "teapot.obj");

	// Sprite
	sprite_ = Sprite::Create(UVCHEKER);

	// particle
	particles_ = new Particles();
	particles_->Initialize();
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	// パーティクルの発生位置を指定
	particles_->SetEmitterPos(Vector3{ 5,5,0 });
	// パーティクルの更新処理
	particles_->Update();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.1f, -100, 100);
	ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::End();

#endif
	sprite_->ImGuiAdjustParameter();
}

void TitleScene::Draw() {
	axis_->Draw(viewProjection_, UVCHEKER);
	plane_->Draw(viewProjection_, UVCHEKER);
	particles_->Draw(viewProjection_, PARTICLE);
	sprite_->Draw();
}

void TitleScene::Finalize() {
	delete axis_;
	delete plane_;
	delete sprite_;
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	delete particles_;
}