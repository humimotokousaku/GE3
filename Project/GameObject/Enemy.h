#pragma once
#include "Line.h"
#include "Collision/Collider.h"

class Enemy : public Collider
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
	Line* line_;
	Camera* camera_;
	Capsule capsule_;
};