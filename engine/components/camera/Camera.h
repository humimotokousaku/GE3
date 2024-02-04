#pragma once
#include "ViewProjection.h"

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
	// viewProjectionの取得
	ViewProjection GetViewProjection() { return viewProjection_; }

	/// Setter
	// カメラの座標をセット
	void SetCameraPos(Vector3 pos) { viewProjection_.translation_ = pos; }

public:
	ViewProjection viewProjection_;
};