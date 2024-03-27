#pragma once
#include "IScene.h"
#include "Input.h"
#include "Sprite.h"

class GameManager;

class TitleScene : public IScene
{
public:
	/// 
	/// Default Method
	/// 

	~TitleScene()override = default;

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
	std::unique_ptr<Sprite> sprite_;
	// テクスチャ
	uint32_t titleNameTexture_;

	Input* input_;
};