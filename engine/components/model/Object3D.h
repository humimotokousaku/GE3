#pragma once
#include "Model.h"
#include "WorldTransform.h"

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
	void Initialize(const std::string& filePath);

	//void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="textureNum">テクスチャ番号</param>
	void Draw(ViewProjection viewProjection, uint32_t textureNum);

public:
	WorldTransform worldTransform;
private:
	Model* model_;
};