#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sphere.h"
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Cube.h"
#include <memory>

class GameManager;

class TitleScene : public IScene
{
public:
	/// 
	/// Default Method
	///  
	
	// 初期化
	void Initialize() override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// 解放処理
	void Finalize()override;

	/// 
	/// User Method
	/// 
	
	// アニメーションの処理
	void AnimUpdate();

private:
	const static int kMaxCube = 2;
	std::unique_ptr<Model> block_;

	int textureNum_;
	Input* input_;
	Vector3 pos_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 角度
	float theta_;
};