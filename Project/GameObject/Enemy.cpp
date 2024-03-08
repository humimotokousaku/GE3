#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Init() {
	line_ = new Line();
	line_->Initialize();
	line_->SetCamera(camera_);
	SetCollisionPrimitive(kCollisionCapsule);
	capsule_ = {
		{-4,0,4},
		{4,0,0},
		1.0f
	};
	SetCapsule(capsule_);
}

void Enemy::Update() {
	if (GetIsOnCollision()) {
		line_->SetRGBA(Vector4{ 1,0,0,1 });
	}
	else {
		line_->SetRGBA(Vector4{ 1,1,1,1 });
	}
}

void Enemy::Draw() {
	line_->Draw(capsule_.start, capsule_.end);
}

void Enemy::OnCollision() {
	//line_->SetRGBA(Vector4{ 1,0,0,1 });
}

Vector3 Enemy::GetWorldPosition() {
	return Vector3{ 0,0,0 };
}