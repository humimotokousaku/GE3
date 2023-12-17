#include "TitleScene.h"
#include "ImGuiManager.h"

void TitleScene::Initialize() {
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	plane_ = Model::CreateModelFromObj("engine/resources", "plane.obj");
	axis_ = Model::CreateModelFromObj("engine/resources", "axis.obj");
	//sprite_ = Sprite::Create(MONSTERBALL);
	//postEffect_ = new PostEffect();
	//postEffect_->Initialize();

	//particles_ = new Particles();
	//particles_->Initialize(true,true);
	//particles_->ShapePlacement(block_);
}

void TitleScene::Update() {
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	//particles_->Update();

	//sprite_->ImGuiAdjustParameter();

	ImGui::Begin("block");
	ImGui::DragFloat3("translation", &plane_->worldTransform.translation_.x, 0.1f, -100, 100);
	ImGui::End();

	ImGui::Begin("axis");
	ImGui::DragFloat3("axis.translation", &axis_->worldTransform.translation_.x, 0.1f, -100, 100);
	ImGui::End();
}

void TitleScene::Draw() {
	//particles_->Draw(viewProjection_, UVCHEKER);
	axis_->Draw(viewProjection_, UVCHEKER);
	plane_->Draw(viewProjection_, UVCHEKER);
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