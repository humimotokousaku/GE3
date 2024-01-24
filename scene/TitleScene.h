#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sphere.h"
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"
#include "../base/Model.h"
#include "../object/Cube.h"
#include "../object/Sprite.h"
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

private:
	// アニメーションの処理
	void AnimUpdate();

	float EaseOutBack(float x);
private:
	const static int kMaxCube = 2;
	std::unique_ptr<Model> block_;

	int textureNum_;
	Input* input_;
	Vector3 pos_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Sprite* titleName_;
	float t_;
	// アニメーションをしているか
	bool isIncrement_;
	int animCount_;

	Sprite* gamePad_A_;
	// 点滅処理用のアルファ
	float alpha_;

	// 背景
	Sprite* backGround_;

	// 角度
	float theta_;
};