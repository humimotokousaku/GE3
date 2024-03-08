#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class Camera
{
public:
	/// 
	/// Default Method
	/// 

	Camera() = default;
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// 
	/// User Method
	/// 
	
	/// Getter
	// worldTransformの取得
	WorldTransform GetWorldTransform() { return worldTransform_; }

	// viewProjectionの取得
	ViewProjection GetViewProjection() { return viewProjection_; }

	/// Setter
	// カメラの座標をセット
	void SetCameraPos(Vector3 pos) { worldTransform_.translation_ = pos; }

public:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};