#pragma once
#include "Line.h"
#include "Collision/Collider.h"
#include "Object3D.h"

class Enemy : public Collider
{
public:
	void Init();
	void Update();
	void Draw(uint32_t textureHandle);

	void SetCamera(Camera* camera) { camera_ = camera; }

	// 衝突応答
	void OnCollision()override;
	// ワールド座標を取得
	Vector3 GetWorldPosition()override;

private:
	Line* line_;
	Object3D* object_;
	Object3D* s_[2];
	cSphere sphere_;
	Camera* camera_;
	Capsule capsule_;
};