#include "Player.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void Player::Init() {
	object_ = new Object3D();

	object_->Initialize();
	object_->SetCamera(camera_);
	object_->SetModel("block.obj");
	SetCollisionPrimitive(kCollisionAABB);
	sphere_ = {
		{0,0. - 0},
		1.0f
	};
	aabb_ = {
		{1.0f,1.0f,1.0f},
		{-1.0f,-1.0f,-1.0f}
	};
	SetAABB(aabb_);
	object_->worldTransform.transform.translate.x = -5;
}

void Player::Update() {
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &object_->worldTransform.transform.translate.x, 0.01f, -100, 100);
	ImGui::End();
}

void Player::Draw() {
	object_->Draw(UVCHEKER);
}

void Player::OnCollision() {

}

Vector3 Player::GetWorldPosition() {
	return object_->worldTransform.transform.translate;
}