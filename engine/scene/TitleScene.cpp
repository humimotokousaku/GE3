#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize() {
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	plane_ = Model::CreateModelFromObj("engine/resources", "plane.obj");
	axis_ = Model::CreateModelFromObj("engine/resources", "teapot.obj");
	//sprite_ = Sprite::Create(MONSTERBALL);
	postEffect_ = new PostEffect();
	postEffect_->Initialize();

	particles_ = new Particles();
	particles_->Initialize(true,true,axis_);
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	particles_->Update();

	//sprite_->ImGuiAdjustParameter();

	ImGui::Begin("plane");
	ImGui::DragFloat3("translation", &plane_->worldTransform.translation_.x, 0.1f, -100, 100);
	ImGui::End();

	ImGui::Begin("axis");
	ImGui::DragFloat3("translation", &axis_->worldTransform.translation_.x, 0.1f, -100, 100);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.1f, -100, 100);
	ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, 0.01f, -6.28f, 6.28f);
	ImGui::End();
}

void TitleScene::Draw() {
	//particles_->Draw(viewProjection_, PARTICLE);
	axis_->Draw(viewProjection_, UVCHEKER);
	plane_->Draw(viewProjection_, UVCHEKER);
	particles_->Draw(viewProjection_, PARTICLE);
	//sprite_->Draw();
	//postEffect_->Draw();
}

void TitleScene::Finalize() {
	delete axis_;
	delete plane_;
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
//	delete sprite_;
	delete particles_;
	delete postEffect_;
}