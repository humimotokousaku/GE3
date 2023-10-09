#pragma once
#include "../../base/ViewProjection.h"
#include "../../base/WorldTransform.h"

class FollowCamera {
public:
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Initialize();

	void Update();

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
};