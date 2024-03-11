#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Init() {
	//line_ = new Line();
	//line_->Initialize();
	//line_->SetCamera(camera_);
	//SetCollisionPrimitive(kCollisionCapsule);
	//capsule_ = {
	//	{0,0,-4},
	//	{0,0,4},
	//	2.0f
	//};
	//SetCapsule(capsule_);

	object_ = new Object3D();

	object_->Initialize();
	object_->SetCamera(camera_);
	object_->SetModel("skydome.obj");
	SetCollisionPrimitive(kCollisionSphere);
	sphere_ = {
		{0,0.0},
		1.0f
	};
	SetSphere(sphere_);
	object_->worldTransform.transform.translate.z = 0;
}

void Enemy::Update() {
	sphere_.center = object_->worldTransform.transform.translate;
	//if (GetIsOnCollision()) {
	//	line_->SetRGBA(Vector4{ 1,0,0,1 });
	//}
	//else {
	//	line_->SetRGBA(Vector4{ 1,1,1,1 });
	//}
}

void Enemy::Draw(uint32_t textureHandle) {
	//line_->Draw(capsule_.start, capsule_.end);
	object_->Draw(textureHandle);
}

void Enemy::OnCollision() {
	//line_->SetRGBA(Vector4{ 1,0,0,1 });
}

Vector3 Enemy::GetWorldPosition() {
	return object_->worldTransform.transform.translate;
}