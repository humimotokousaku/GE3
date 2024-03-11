#pragma once
#include"Object3D.h"
#include "Collision/Collider.h"

class Player : public Collider
{
public:
	void Init();
	void Update();
	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }

	// 衝突応答
	void OnCollision()override;
	// ワールド座標を取得
	Vector3 GetWorldPosition()override;
private:
	Object3D* object_;
	Camera* camera_;
	cSphere sphere_;
	cAABB aabb_;
};