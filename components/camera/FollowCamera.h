#pragma once
#include "../../base/ViewProjection.h"
#include "../../base/WorldTransform.h"

class FollowCamera {
public:
	void SetTarget(const WorldTransform* target) { 
		target_ = target;
		Reset();
	}

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Initialize();

	void Update();

	// 追従対象をセットしたら呼び出す初期化
	void Reset();

	// 追従対象からのオフセットを計算する
	Vector3 TargetOffset() const;

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	float destinationAngleY_ = 0.0f;
};