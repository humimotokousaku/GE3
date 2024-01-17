#pragma once
#include "../base/Model.h"
#include "../base/WorldTransform.h"
#include "../math/Vector3.h"

class RailCamera {
public:
	ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform(){ return worldTransform_; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform worldTransform, const Vector3& radian);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Vector3 target);

	void SetTranslation(Vector3 pos) { worldTransform_.translation_ = pos; }
	void SetViewTranslation(Vector3 pos) { viewProjection_.translation_ = pos; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};