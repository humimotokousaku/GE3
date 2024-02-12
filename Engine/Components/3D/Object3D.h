#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "ModelManager.h"

class Object3D
{
public:
	///
	/// Default Method
	/// 
	
	// コンストラクタ
	Object3D() = default;
	// デストラクタ
	~Object3D();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="textureNum">テクスチャ番号</param>
	void Draw(uint32_t textureNum);

	///
	/// Default Method
	/// 

	/// Getter


	/// Setter
	// カメラ
	void SetCamera(Camera* camera) { camera_ = camera; }

	// カメラをセットしていないとき用
	void SetSubCamera(ViewProjection viewProjection) { this->viewProjection = viewProjection; }

	// モデルのセット
	void SetModel(const std::string& filePath) { model_ = ModelManager::GetInstance()->FindModel(filePath); }

public:
	WorldTransform worldTransform;
	ViewProjection viewProjection;
	Camera* camera_;

private:
	Model* model_;
};